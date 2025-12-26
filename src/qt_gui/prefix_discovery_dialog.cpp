#include "prefix_discovery_dialog.h"
#include <QFileDialog>

PrefixDiscoveryDialog::PrefixDiscoveryDialog(QWidget *parent)
    : QDialog(parent) {
  ui.setupUi(this);
  refreshList();
}

void PrefixDiscoveryDialog::refreshList() {
  discovered = PrefixDiscovery::discover();
  ui.prefixList->clear();
  for (const auto &p : discovered) {
    QString text =
        QString::fromUtf8("[%1] %2 (%3)").arg(p.type, p.name, p.path);
    ui.prefixList->addItem(text);
  }
}

QString PrefixDiscoveryDialog::getSelectedPath() const { return selectedPath; }

void PrefixDiscoveryDialog::on_browseButton_clicked() {
  QString dir = QFileDialog::getExistingDirectory(
      this, tr("Select Wine Prefix"), QDir::homePath(),
      QFileDialog::ShowDirsOnly);
  if (!dir.isEmpty()) {
    selectedPath = dir;
    accept();
  }
}

void PrefixDiscoveryDialog::on_selectButton_clicked() {
  int row = ui.prefixList->currentRow();
  if (row >= 0 && row < discovered.size()) {
    selectedPath = discovered[row].path;
    accept();
  }
}

void PrefixDiscoveryDialog::on_cancelButton_clicked() { reject(); }

void PrefixDiscoveryDialog::on_prefixList_itemDoubleClicked() {
  on_selectButton_clicked();
}
