#include "log_dock.h"
#include "main_window.h"
#include "main_menubar.h"
#include "main_toolbar.h"
#include "main_central_widget.h"
#include <log4cplus/log4cplus.h>
#include <QIcon>


using namespace gump;


MainWindow::MainWindow(QWidget *parent)
  : QMainWindow(parent)
{
  setWindowTitle("Gump");
  setWindowIcon(QIcon(":/icon.png"));

  log_dock_ = new LogDock(this);
  addDockWidget(Qt::BottomDockWidgetArea, log_dock_);

  menu_ = new MainMenuBar(this);
  toolbar_ = new MainToolBar(this);
  central_widget_ = new MainCentralWidget();

  addToolBar(toolbar_);
  setCentralWidget(central_widget_);

  log_appender_ = log4cplus::SharedAppenderPtr(new LogDockAppender(log_dock_));
  log4cplus::Logger::getInstance(LOGGER_NAME).addAppender(log_appender_);

  setWindowState(Qt::WindowMaximized);
  connect(toolbar_, SIGNAL(SearchItem(std::string,std::string,std::string)),
          this, SLOT(SearchItem(std::string,std::string,std::string)));
  connect(toolbar_, SIGNAL(PlayStream()), this, SLOT(PlayStream()));
  connect(toolbar_, SIGNAL(PauseStream()), this, SLOT(PauseStream()));
  connect(toolbar_, SIGNAL(StopStream()), this, SLOT(StopStream()));
  connect(toolbar_, SIGNAL(ShowDetails()), this, SLOT(ShowDetails()));
}


MainWindow::~MainWindow()
{

}

void MainWindow::UpdatePreferences()
{
  central_widget_->UpdateStreamRule();
}

void MainWindow::SearchItem(std::string vid, std::string cname,
                            std::string stream)
{
  central_widget_->SearchItem(vid, cname, stream);
}

void MainWindow::PlayStream()
{
  central_widget_->PlayStream();
}

void MainWindow::PauseStream()
{
  central_widget_->PauseStream();
}

void MainWindow::StopStream()
{
  central_widget_->StopStream();
}

void MainWindow::ShowDetails()
{
  central_widget_->ShowDetails();
}

void MainWindow::closeEvent(QCloseEvent *)
{
  log_appender_->destructorImpl();
  log4cplus::Logger::getInstance(LOGGER_NAME).removeAllAppenders();
}

void MainWindow::moveEvent(QMoveEvent *)
{
  central_widget_->WindowMove();
}


