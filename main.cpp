/*
    <one line to give the program's name and a brief idea of what it does.>
    Copyright (C) 2017  Nick Korotysh <nick.korotysh@gmail.com>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "mainwindow.h"
#include "application.h"

int main(int argc, char *argv[])
{
  QApplication::setApplicationDisplayName("SeeX");
  QApplication::setApplicationName("SeeX");
  QApplication::setApplicationVersion("1.0.5.130");
  QApplication::setOrganizationName("Nick Korotysh");
  QApplication::setOrganizationDomain("seex.kolcha.github.io");

  QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);

  Application a(argc, argv);
  MainWindow w;
  QObject::connect(&a, &Application::fileOpened, &w, &MainWindow::openFile);
  w.showMaximized();

  return a.exec();
}
