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

#ifndef __TrenchBroom__MapFrame__
#define __TrenchBroom__MapFrame__

#include "Model/MapFormat.h"
#include "Model/ModelTypes.h"
#include "View/ViewTypes.h"

#include <wx/frame.h>

class wxTimer;
class wxTimerEvent;

namespace TrenchBroom {
    class Logger;

    namespace IO {
        class Path;
    }

    namespace View {
        class Autosaver;
        class Console;
        class FrameManager;
        class GLContextManager;
        class Inspector;
        class SwitchableMapViewContainer;

        class MapFrame : public wxFrame {
        private:
            FrameManager* m_frameManager;
            MapDocumentSPtr m_document;

            Autosaver* m_autosaver;
            wxTimer* m_autosaveTimer;

            GLContextManager* m_contextManager;
            SwitchableMapViewContainer* m_mapView;
            Console* m_console;
            Inspector* m_inspector;

            wxWindow* m_lastFocus;
        public:
            MapFrame();
            MapFrame(FrameManager* frameManager, MapDocumentSPtr document);
            void Create(FrameManager* frameManager, MapDocumentSPtr document);
            ~MapFrame();

            void positionOnScreen(wxFrame* reference);
        public: // getters and such
            Logger* logger() const;
        public: // drop targets
            void setToolBoxDropTarget();
            void clearDropTarget();
        public: // document management
            bool newDocument(Model::GamePtr game, Model::MapFormat::Type mapFormat);
            bool openDocument(Model::GamePtr game, const IO::Path& path);
        private:
            bool saveDocument();
            bool saveDocumentAs();

            bool confirmOrDiscardChanges();
        private: // title bar contents
            void updateTitle();
        private: // menu bar
            void OnChildFocus(wxChildFocusEvent& event);
            void rebuildMenuBar();
            void createMenuBar();
            void addRecentDocumentsMenu(wxMenuBar* menuBar);
            void removeRecentDocumentsMenu(wxMenuBar* menuBar);
            void updateRecentDocumentsMenu();
        private: // gui creation
            void createGui();
        private: // notification handlers
            void bindObservers();
            void unbindObservers();

            void documentWasCleared(View::MapDocument* document);
            void documentDidChange(View::MapDocument* document);
            void documentModificationStateDidChange();
            void preferenceDidChange(const IO::Path& path);
        private: // menu event handlers
            void bindEvents();

            void OnFileSave(wxCommandEvent& event);
            void OnFileSaveAs(wxCommandEvent& event);
            void OnFileLoadPointFile(wxCommandEvent& event);
            void OnFileUnloadPointFile(wxCommandEvent& event);
            void OnFileClose(wxCommandEvent& event);

            void OnEditUndo(wxCommandEvent& event);
            void OnEditRedo(wxCommandEvent& event);
            void OnEditRepeat(wxCommandEvent& event);
            void OnEditClearRepeat(wxCommandEvent& event);

            void OnEditCut(wxCommandEvent& event);
            void OnEditCopy(wxCommandEvent& event);
            void copyToClipboard();

            void OnEditPaste(wxCommandEvent& event);
            void OnEditPasteAtOriginalPosition(wxCommandEvent& event);
            bool paste();

            void OnEditDelete(wxCommandEvent& event);
            void OnEditDuplicate(wxCommandEvent& event);

            void OnEditSelectAll(wxCommandEvent& event);
            void OnEditSelectSiblings(wxCommandEvent& event);
            void OnEditSelectTouching(wxCommandEvent& event);
            void OnEditSelectInside(wxCommandEvent& event);
            void OnEditSelectByLineNumber(wxCommandEvent& event);
            void OnEditSelectNone(wxCommandEvent& event);

            void OnEditGroupSelectedObjects(wxCommandEvent& event);
            void OnEditUngroupSelectedObjects(wxCommandEvent& event);

            void OnEditHideSelectedObjects(wxCommandEvent& event);
            void OnEditIsolateSelectedObjects(wxCommandEvent& event);
            void OnEditShowHiddenObjects(wxCommandEvent& event);

            void OnEditDeactivateTool(wxCommandEvent& event);
            void OnEditToggleCreateBrushTool(wxCommandEvent& event);
            void OnEditToggleClipTool(wxCommandEvent& event);
            void OnEditToggleRotateObjectsTool(wxCommandEvent& event);
            void OnEditToggleVertexTool(wxCommandEvent& event);

            void OnEditCreateBrushFromConvexHull(wxCommandEvent& event);

            void OnEditReplaceTexture(wxCommandEvent& event);

            void OnEditToggleTextureLock(wxCommandEvent& event);

            void OnEditSnapVertices(wxCommandEvent& event);

            void OnViewToggleShowGrid(wxCommandEvent& event);
            void OnViewToggleSnapToGrid(wxCommandEvent& event);
            void OnViewIncGridSize(wxCommandEvent& event);
            void OnViewDecGridSize(wxCommandEvent& event);
            void OnViewSetGridSize(wxCommandEvent& event);

            void OnViewMoveCameraToNextPoint(wxCommandEvent& event);
            void OnViewMoveCameraToPreviousPoint(wxCommandEvent& event);
            void OnViewCenterCameraOnSelection(wxCommandEvent& event);
            void OnViewMoveCameraToPosition(wxCommandEvent& event);

            void OnViewSwitchToMapInspector(wxCommandEvent& event);
            void OnViewSwitchToEntityInspector(wxCommandEvent& event);
            void OnViewSwitchToFaceInspector(wxCommandEvent& event);

            void OnFlipObjectsHorizontally(wxCommandEvent& event);
            void OnFlipObjectsVertically(wxCommandEvent& event);
            
            void OnUpdateUI(wxUpdateUIEvent& event);
        private:
            bool canLoadPointFile() const;
            bool canUnloadPointFile() const;
            bool canUndo() const;
            bool canRedo() const;
            bool canCut() const;
            bool canCopy() const;
            bool canPaste() const;
            bool canDelete() const;
            bool canDuplicate() const;
            bool canSelectSiblings() const;
            bool canSelectByBrush() const;
            bool canSelect() const;
            bool canDeselect() const;
            bool canChangeSelection() const;
            bool canGroup() const;
            bool canUngroup() const;
            bool canHide() const;
            bool canIsolate() const;
            bool canCreateConvexHull() const;
            bool canSnapVertices() const;
            bool canDecGridSize() const;
            bool canIncGridSize() const;
            bool canMoveCameraToNextPoint() const;
            bool canMoveCameraToPreviousPoint() const;
            bool canCenterCamera() const;
        private: // other event handlers
            void OnClose(wxCloseEvent& event);
            void OnAutosaveTimer(wxTimerEvent& event);
        };
    }
}

#endif /* defined(__TrenchBroom__MapFrame__) */
