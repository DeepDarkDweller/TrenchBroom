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

#ifndef __TrenchBroom__EntityRenderer__
#define __TrenchBroom__EntityRenderer__

#include "AttrString.h"
#include "Color.h"
#include "Model/ModelTypes.h"
#include "Renderer/EdgeRenderer.h"
#include "Renderer/EntityModelRenderer.h"
#include "Renderer/FontDescriptor.h"
#include "Renderer/Renderable.h"
#include "Renderer/TextRenderer.h"
#include "Renderer/TriangleRenderer.h"
#include "Renderer/Vbo.h"

#include <map>

namespace TrenchBroom {
    namespace Assets {
        class EntityModelManager;
    }
    
    namespace Model {
        class EditorContext;
    }
    
    namespace Renderer {
        class FontManager;
        class RenderBatch;
        class RenderContext;
        class Vbo;
        
        class EntityRenderer {
        private:
            typedef Model::Entity* Key;
            typedef TextRenderer<Key> ClassnameRenderer;
            
            class EntityClassnameAnchor : public TextAnchor {
            private:
                const Model::Entity* m_entity;
            protected:
                Vec3f basePosition() const;
                Alignment::Type alignment() const;
            public:
                EntityClassnameAnchor(const Model::Entity* entity);
            };
            
            class EntityClassnameFilter : public ClassnameRenderer::TextRendererFilter {
            private:
                const Model::EditorContext& m_editorContext;
                bool m_showHiddenEntities;
            public:
                EntityClassnameFilter(const Model::EditorContext& editorContext, bool showHiddenEntities);
                bool stringVisible(RenderContext& renderContext, const Key& entity) const;
            };

            class EntityClassnameColorProvider : public ClassnameRenderer::TextColorProvider {
            private:
                const Color& m_textColor;
                const Color& m_backgroundColor;
            public:
                EntityClassnameColorProvider(const Color& textColor, const Color& backgroundColor);
                
                Color textColor(RenderContext& renderContext, const Key& entity) const;
                Color backgroundColor(RenderContext& renderContext, const Key& entity) const;
            };
            
            const Model::EditorContext& m_editorContext;
            Model::EntitySet m_entities;
            
            EdgeRenderer m_wireframeBoundsRenderer;
            TriangleRenderer m_solidBoundsRenderer;
            ClassnameRenderer m_classnameRenderer;
            EntityModelRenderer m_modelRenderer;
            bool m_boundsValid;
            
            Color m_overlayTextColor;
            Color m_overlayBackgroundColor;
            bool m_showOccludedOverlays;
            bool m_tint;
            Color m_tintColor;
            bool m_overrideBoundsColor;
            Color m_boundsColor;
            bool m_showOccludedBounds;
            Color m_occludedBoundsColor;
            bool m_showAngles;
            Color m_angleColor;
            bool m_showHiddenEntities;
            
            Vbo m_vbo;
        public:
            EntityRenderer(Assets::EntityModelManager& entityModelManager, const Model::EditorContext& editorContext);
            ~EntityRenderer();

            void addEntity(Model::Entity* entity);
            void updateEntity(Model::Entity* entity);
            void removeEntity(Model::Entity* entity);
            void invalidate();
            void clear();
            void reloadModels();

            template <typename Iter>
            void addEntities(Iter cur, const Iter end) {
                while (cur != end) {
                    addEntity(*cur);
                    ++cur;
                }
            }
            template <typename Iter>
            void updateEntities(Iter cur, const Iter end) {
                while (cur != end) {
                    updateEntity(*cur);
                    ++cur;
                }
            }
            
            template <typename Iter>
            void removeEntities(Iter cur, const Iter end) {
                while (cur != end) {
                    removeEntity(*cur);
                    ++cur;
                }
            }
            
            void setOverlayTextColor(const Color& overlayTextColor);
            void setOverlayBackgroundColor(const Color& overlayBackgroundColor);
            void setShowOccludedOverlays(bool showOccludedOverlays);
            
            void setTint(bool tint);
            void setTintColor(const Color& tintColor);
            
            void setOverrideBoundsColor(bool overrideBoundsColor);
            void setBoundsColor(const Color& boundsColor);
            
            void setShowOccludedBounds(bool showOccludedBounds);
            void setOccludedBoundsColor(const Color& occludedBoundsColor);
            
            void setShowAngles(bool showAngles);
            void setAngleColor(const Color& angleColor);
            
            void setShowHiddenEntities(bool showHiddenEntities);
        public: // rendering
            void render(RenderContext& renderContext, RenderBatch& renderBatch);
        private:
            void renderBounds(RenderContext& renderContext, RenderBatch& renderBatch);
            void renderWireframeBounds(RenderBatch& renderBatch);
            void renderSolidBounds(RenderBatch& renderBatch);
            void renderModels(RenderContext& renderContext, RenderBatch& renderBatch);
            void renderClassnames(RenderContext& renderContext, RenderBatch& renderBatch);
            void renderAngles(RenderContext& renderContext, RenderBatch& renderBatch);
            Vec3f::List arrowHead(float length, float width) const;
            static FontDescriptor font();
            void invalidateBounds();
            void validateBounds();
            
            AttrString entityString(const Model::Entity* entity) const;
            const Color& boundsColor(const Model::Entity& entity) const;
        };
    }
}

#endif /* defined(__TrenchBroom__EntityRenderer__) */
