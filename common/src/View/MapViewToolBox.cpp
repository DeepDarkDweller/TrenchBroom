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

#include "MapViewToolBox.h"
#include "View/MovementRestriction.h"
#include "View/MoveObjectsTool.h"
#include "View/RotateObjectsTool.h"
#include "View/SelectionTool.h"
#include "View/VertexTool.h"

namespace TrenchBroom {
    namespace View {
        MapViewToolBox::MapViewToolBox(MapDocumentWPtr document, wxBookCtrlBase* bookCtrl) :
        m_movementRestriction(new MovementRestriction()),
        m_moveObjectsTool(NULL),
        m_rotateObjectsTool(NULL),
        m_selectionTool(NULL),
        m_vertexTool(NULL) {
            createTools(document, bookCtrl);
            bindObservers();
        }
        
        MapViewToolBox::~MapViewToolBox() {
            unbindObservers();
            destroyTools();
            delete m_movementRestriction;
        }

        const MovementRestriction& MapViewToolBox::movementRestriction() const {
            return *m_movementRestriction;
        }
        
        MovementRestriction& MapViewToolBox::movementRestriction() {
            return *m_movementRestriction;
        }

        MoveObjectsTool* MapViewToolBox::moveObjectsTool() {
            return m_moveObjectsTool;
        }
        
        RotateObjectsTool* MapViewToolBox::rotateObjectsTool() {
            return m_rotateObjectsTool;
        }
        
        SelectionTool* MapViewToolBox::selectionTool() {
            return m_selectionTool;
        }
        
        VertexTool* MapViewToolBox::vertexTool() {
            return m_vertexTool;
        }

        void MapViewToolBox::toggleRotateObjectsTool() {
            toggleTool(m_rotateObjectsTool);
        }
        
        bool MapViewToolBox::rotateObjectsToolActive() const {
            return toolActive(m_rotateObjectsTool);
        }

        double MapViewToolBox::rotateToolAngle() const {
            assert(rotateObjectsToolActive());
            return m_rotateObjectsTool->angle();
        }
        
        const Vec3 MapViewToolBox::rotateToolCenter() const {
            assert(rotateObjectsToolActive());
            return m_rotateObjectsTool->center();
        }

        void MapViewToolBox::moveRotationCenter(const Vec3& delta) {
            assert(rotateObjectsToolActive());
            m_rotateObjectsTool->moveCenter(delta);
        }

        void MapViewToolBox::toggleVertexTool() {
            toggleTool(m_vertexTool);
        }
        
        bool MapViewToolBox::vertexToolActive() const {
            return toolActive(m_vertexTool);
        }

        void MapViewToolBox::createTools(MapDocumentWPtr document, wxBookCtrlBase* bookCtrl) {
            m_moveObjectsTool = new MoveObjectsTool(document, *m_movementRestriction);
            m_rotateObjectsTool = new RotateObjectsTool(document, *m_movementRestriction);
            m_selectionTool = new SelectionTool(document);
            m_vertexTool = new VertexTool(document, *m_movementRestriction);
            
            deactivateWhen(m_rotateObjectsTool, m_moveObjectsTool);
            deactivateWhen(m_vertexTool, m_moveObjectsTool);
            
            m_moveObjectsTool->createPage(bookCtrl);
            m_rotateObjectsTool->createPage(bookCtrl);
        }
        
        void MapViewToolBox::destroyTools() {
            delete m_moveObjectsTool;
            delete m_rotateObjectsTool;
            delete m_selectionTool;
            delete m_vertexTool;
        }
        
        void MapViewToolBox::bindObservers() {
            toolActivatedNotifier.addObserver(this, &MapViewToolBox::toolActivated);
            toolDeactivatedNotifier.addObserver(this, &MapViewToolBox::toolDeactivated);
        }
        
        void MapViewToolBox::unbindObservers() {
            toolActivatedNotifier.removeObserver(this, &MapViewToolBox::toolActivated);
            toolDeactivatedNotifier.removeObserver(this, &MapViewToolBox::toolDeactivated);
        }
        
        void MapViewToolBox::toolActivated(Tool* tool) {
            if (tool == m_rotateObjectsTool)
                m_rotateObjectsTool->showPage();
        }
        
        void MapViewToolBox::toolDeactivated(Tool* tool) {
            m_moveObjectsTool->showPage();
        }
    }
}
