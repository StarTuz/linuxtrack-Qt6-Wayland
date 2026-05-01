#ifndef PROFILE_SELECTOR__H
#define PROFILE_SELECTOR__H

#include "ui_profile_selector.h"
#include <QWidget>

class ProfileSetup;

class ProfileSelector : public QWidget {
  Q_OBJECT
public:
  ProfileSelector(QWidget *parent = 0);
  ~ProfileSelector();
  void refresh();
  bool selectProfile(const QString &profileName, bool createIfMissing = false);

private:
  Ui::ProfileSelectorForm ui;
  ProfileSetup *ps;
  bool initializing;
  bool setCurrentProfile(const QString &prof);
signals:
  void profileChanged(const QString &profileName);
public slots:
  bool close();
private slots:
  void profilesIndexChanged(int index);
  void on_CopyFromDefault_pressed();
  void on_ImportProfile_pressed();
  void on_ExportProfile_pressed();
  void on_NewProfile_pressed();
};

#endif
