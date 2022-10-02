void MainWindow::parseCities()
{
    QString path = QFileDialog::getOpenFileName (this, "", "",  "Json Document (*.json)");
    if (path == "") return;
    QFile jsonFile (path);
    jsonFile.open (QIODevice::ReadOnly);
    if (!jsonFile.isOpen()) return;
    QJsonDocument doc = QJsonDocument::fromJson (jsonFile.readAll());
    jsonFile.close();
    QJsonArray arr = doc.array();
    QJsonArray n_arr;
    for (int i = 0; i < arr.count(); ++i) {
        auto obj = arr[i].toObject();
        if (obj.contains ("country")) {
            if (obj["country"] == "RU") {
                n_arr.append (arr[i]);
            }
        }
    }
    QString n_path = QFileDialog::getSaveFileName (this, "", "", "Text file (*.txt)");
    if (n_path == "") return;
    QFile n_file (n_path);
    n_file.open (QIODevice::WriteOnly);
    QJsonDocument n_doc (n_arr);
    if (!n_file.isOpen()) return;
    n_file.write (n_doc.toJson());
    n_file.close();
}