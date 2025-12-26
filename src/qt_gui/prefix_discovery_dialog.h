#ifndef PREFIX_DISCOVERY_DIALOG_H
#define PREFIX_DISCOVERY_DIALOG_H

#include "prefix_discovery.h"
#include "ui_prefix_discovery_dialog.h"
#include <QDialog>

class PrefixDiscoveryDialog : public QDialog {
  Q_OBJECT
public:
  explicit PrefixDiscoveryDialog(QWidget *parent = nullptr);
  QString getSelectedPath() const;

private slots:
  void on_browseButton_clicked();
  void on_selectButton_clicked();
  void on_cancelButton_clicked();
  void on_prefixList_itemDoubleClicked();

private:
  Ui::PrefixDiscoveryDialog ui;
  QList<DetectedPrefix> discovered;
  QString selectedPath;
  void refreshList();
};

#endif
