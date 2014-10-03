/*
 Copyright (C) 2010-2014 Kristian Duske
 
 This file is part of TrenchBroom.
 
 TrenchBroom is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.
 
 TrenchBroom is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with TrenchBroom. If not, see <http://www.gnu.org/licenses/>.
 */

#include "EntityModelRenderer.h"

#include "TrenchBroom.h"
#include "PreferenceManager.h"
#include "Preferences.h"
#include "VecMath.h"
#include "CollectionUtils.h"
#include "Assets/EntityModel.h"
#include "Assets/EntityModelManager.h"
#include "Model/EditorContext.h"
#include "Model/Entity.h"
#include "Renderer/MeshRenderer.h"
#include "Renderer/RenderContext.h"
#include "Renderer/ShaderManager.h"
#include "Renderer/Transformation.h"

namespace TrenchBroom {
    namespace Renderer {
        EntityModelRenderer::EntityModelRenderer(Assets::EntityModelManager& entityModelManager, const Model::EditorContext& editorContext) :
        m_entityModelManager(entityModelManager),
        m_editorContext(editorContext),
        m_showHiddenEntities(false) {}

        EntityModelRenderer::~EntityModelRenderer() {
            clear();
        }
        
        void EntityModelRenderer::addEntity(Model::Entity* entity) {
            const Assets::ModelSpecification modelSpec = entity->modelSpecification();
            Assets::EntityModel* model = m_entityModelManager.model(modelSpec.path);
            if (model != NULL) {
                if (!model->prepared())
                    m_unpreparedModels.push_back(model);
                MeshRenderer* renderer = m_entityModelManager.renderer(modelSpec);
                if (renderer != NULL) {
                    m_entities[entity] = renderer;
                    if (!renderer->prepared())
                        m_unpreparedRenderers.push_back(renderer);
                }
            }
        }
        
        void EntityModelRenderer::updateEntity(Model::Entity* entity) {
            removeEntity(entity);
            addEntity(entity);
        }
        
        void EntityModelRenderer::removeEntity(Model::Entity* entity) {
            m_entities.erase(entity);
        }

        void EntityModelRenderer::clear() {
            m_entities.clear();
            m_unpreparedModels.clear();
            m_unpreparedRenderers.clear();
        }

        bool EntityModelRenderer::applyTinting() const {
            return m_applyTinting;
        }
        
        void EntityModelRenderer::setApplyTinting(const bool applyTinting) {
            m_applyTinting = applyTinting;
        }
        
        const Color& EntityModelRenderer::tintColor() const {
            return m_tintColor;
        }
        
        void EntityModelRenderer::setTintColor(const Color& tintColor) {
            m_tintColor = tintColor;
        }
        
        bool EntityModelRenderer::showHiddenEntities() const {
            return m_showHiddenEntities;
        }
        
        void EntityModelRenderer::setShowHiddenEntities(const bool showHiddenEntities) {
            m_showHiddenEntities = showHiddenEntities;
        }

        void EntityModelRenderer::render(RenderBatch& renderBatch) {
            renderBatch.add(this);
        }

        void EntityModelRenderer::doPrepare(Vbo& vbo) {
            prepareModels();
            prepareRenderers(vbo);
        }
        
        void EntityModelRenderer::prepareModels() {
            EntityModelList::const_iterator it, end;
            for (it = m_unpreparedModels.begin(), end = m_unpreparedModels.end(); it != end; ++it) {
                Assets::EntityModel* model = *it;
                model->prepare();
            }
            m_unpreparedModels.clear();
        }
        
        void EntityModelRenderer::prepareRenderers(Vbo& vbo) {
            RendererList::const_iterator it, end;
            for (it = m_unpreparedRenderers.begin(), end = m_unpreparedRenderers.end(); it != end; ++it) {
                MeshRenderer* renderer = *it;
                renderer->prepare(vbo);
            }
            m_unpreparedRenderers.clear();
        }

        void EntityModelRenderer::doRender(RenderContext& renderContext) {
            PreferenceManager& prefs = PreferenceManager::instance();
            
            ActiveShader shader(renderContext.shaderManager(), Shaders::EntityModelShader);
            shader.set("Brightness", prefs.get(Preferences::Brightness));
            shader.set("ApplyTinting", m_applyTinting);
            shader.set("TintColor", m_tintColor);
            shader.set("GrayScale", false);
            shader.set("Texture", 0);
            
            glEnable(GL_TEXTURE_2D);
            glActiveTexture(GL_TEXTURE0);
            
            EntityMap::iterator it, end;
            for (it = m_entities.begin(), end = m_entities.end(); it != end; ++it) {
                Model::Entity* entity = it->first;
                if (!m_showHiddenEntities && !m_editorContext.visible(entity))
                    continue;
                
                MeshRenderer* renderer = it->second;
                
                const Vec3f position = entity->origin();
                const Quatf rotation = entity->rotation();
                const Mat4x4f matrix = translationMatrix(position) * rotationMatrix(rotation);
                MultiplyModelMatrix multMatrix(renderContext.transformation(), matrix);
                
                renderer->render();
            }
        }
    }
}
