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
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QFileInfo>

#include "fileprovider.h"
#include "frameprovider.h"
#include "statuslabel.h"

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  ui->setupUi(this);

  ui->scrollArea->setBackgroundRole(QPalette::Dark);

  ui->actionOpen->setShortcut(QKeySequence::Open);

  fi_provider_ = new FileProvider(this);
  connect(ui->actionNextFile, &QAction::triggered, fi_provider_, &FileProvider::nextFile);
  connect(ui->actionPreviousFile, &QAction::triggered, fi_provider_, &FileProvider::previousFile);
  connect(ui->actionFirstFile, &QAction::triggered, fi_provider_, &FileProvider::firstFile);
  connect(ui->actionLastFile, &QAction::triggered, fi_provider_, &FileProvider::lastFile);

  fr_provider_ = new FrameProvider(this);
  connect(fi_provider_, &FileProvider::currentFileChanged, fr_provider_, &FrameProvider::setFileName);
  connect(ui->actionNextFrame, &QAction::triggered, fr_provider_, &FrameProvider::nextFrame);
  connect(ui->actionPreviousFrame, &QAction::triggered, fr_provider_, &FrameProvider::previousFrame);
  connect(fr_provider_, &FrameProvider::fileNameChanged, this, &MainWindow::setWindowFilePath);
  connect(fr_provider_, &FrameProvider::currentFrameChanged, ui->label, &ImageLabel::setImage);
  connect(fr_provider_, &FrameProvider::currentFrameChanged, this, &MainWindow::updateNavigationActions);

  st_file_ = new StatusLabel(":/images/status/folder.svg", "0/0");
  st_frame_ = new StatusLabel(":/images/status/frame.svg", "0/0");
  st_zoom_ = new StatusLabel(":/images/status/zoom.svg", "100 %");
  st_resolution_ = new StatusLabel(":/images/status/resolution.svg", "0x0");
  st_format_ = new StatusLabel(":/images/status/file.svg", "-");
  ui->statusBar->addWidget(st_file_);
  ui->statusBar->addWidget(st_frame_);
  ui->statusBar->addPermanentWidget(st_zoom_);
  ui->statusBar->addPermanentWidget(st_resolution_);
  ui->statusBar->addPermanentWidget(st_format_);
}

MainWindow::~MainWindow()
{
  delete ui;
}

void MainWindow::openFile(const QString& filename)
{
  QFileInfo fi(filename);
  fi_provider_->scanDir(fi.absolutePath());
  fi_provider_->setCurrentFile(fi.absoluteFilePath());
}

void MainWindow::on_actionOpen_triggered()
{
  QString filename = QFileDialog::getOpenFileName(this);
  if (!filename.isEmpty()) openFile(filename);
}

void MainWindow::updateNavigationActions()
{
  ui->actionNextFile->setDisabled(fi_provider_->currentIndex() == fi_provider_->filesCount() - 1);
  ui->actionPreviousFile->setDisabled(fi_provider_->currentIndex() == 0);
  ui->actionFirstFile->setEnabled(ui->actionPreviousFile->isEnabled());
  ui->actionLastFile->setEnabled(ui->actionNextFile->isEnabled());
  int frames_count = fr_provider_->framesCount();
  ui->actionNextFrame->setDisabled(frames_count <= 1 || fr_provider_->currentIndex() == frames_count - 1);
  ui->actionPreviousFrame->setDisabled(frames_count <= 1 || fr_provider_->currentIndex() == 0);
}
