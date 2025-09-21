//
// Created by gabriel on 23.08.25.
//

#ifndef FXRUNNER_HISTORYVIEW_H
#define FXRUNNER_HISTORYVIEW_H
#include <core.h>


class HistoryView : public FXGHI::FXSecondaryWindow {
FXDECLARE( HistoryView )
  Application *m_app;
  FXTopWindow *m_parent;

  FXIconList *m_view;     // List of history items

  //FXButton   *m_select;   // Vybrat polozku
  FXButton   *m_remove;   // Odstranit
  FXButton   *m_clear;    // Smazat vse
  FXButton   *m_up;       // Posunout vyse


public :
  HistoryView( FXTopWindow *w );
  ~HistoryView( ) override;

  /* Access methods */
  void create( ) override;

  /* Operations methods */
  FXuint execute( FXuint placement = PLACEMENT_CURSOR ) override;

  /* Messagess & handlers */
  enum {
    ID_REFRESH = FXSecondaryWindow::ID_LAST,   // Aktualize of history list
    ID_SELECT,                                 // Select record from history
    ID_REMOVE,                                 // Remove select record(s) from history
    ID_CLEAR,                                  // Remove all records from history
    ID_TOP,                                     // Move record(s) up
    ID_HELP,                                   // Show help from this window
    ID_LAST,
  };
  long on_List( FXObject *sender, FXSelector selector, void *ptr );
  long on_Window( FXObject *sender, FXSelector selector, void *ptr );

protected:
  HistoryView( ) = default;
  FXString TaskToString( Task *task );
  Task*    TaskFromString( const FXString &str, Task *task = nullptr );
  void     UpdateList( );
};


#endif //FXRUNNER_HISTORYVIEW_H