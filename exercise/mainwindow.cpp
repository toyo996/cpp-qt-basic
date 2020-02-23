#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->listWidget->setViewMode(QListWidget::IconMode);
    ui->listWidget->setIconSize(QSize(200,150));            //ova seto e za slikite, goleminata
    ui->listWidget->setResizeMode(QListWidget::Adjust);


    const QString folderPath = QFileDialog::getExistingDirectory(this, tr("Image folder"));

    ui->label->setText(folderPath);

    if(!folderPath.isEmpty())
    {
        QDir dir(folderPath);
        QStringList filter;
        filter << QLatin1String("*.png");
        filter << QLatin1String("*.jpg");
        dir.setNameFilters(filter);
        QFileInfoList filelistinfo = dir.entryInfoList();       //ovdeka gi prikazuvam slikite mali vo prozoreco
        QStringList fileList;
        foreach (const QFileInfo &fileinfo, filelistinfo) {
            QString imageFile = fileinfo.absoluteFilePath();
            QListWidgetItem *item=new QListWidgetItem(QIcon (imageFile),QString(imageFile));
            ui->listWidget->addItem(item);
        }
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
    QString name = ui->textEdit->toPlainText();
    QString birth = ui->textEdit_2->toPlainText();     //zemam podatoci od prozorcinata
    QString email = ui->textEdit_3->toPlainText();

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("C:/proba2.DB");
    if (!db.open())
    {
        QMessageBox::critical(nullptr, QObject::tr("Cannot open database"),
            QObject::tr("Unable to establish a database connection.\n"
                        "This example needs SQLite support. Please read "       //nekoe cudo od internet e ova
                        "the Qt SQL driver documentation for information how "  //vo vrska so bazata
                        "to build it.\n\n"
                        "Click Cancel to exit."), QMessageBox::Cancel);
    }


    // KREIRAM TABELA OVDE I JA POLNAM SO VREDNOSTI
    QSqlQuery query;
    query.exec(
                "create table myTable( `ID`	INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, `Name` varchar(20),`Birthday` varchar(20),`email` varchar(40),`Path` varchar(200))"
                );

    QString path=ui->label->text();
    query.exec("insert into myTable (Name, Birthday, email, Path)"
                  "values('"+name+"','"+birth+"','"+email+"','"+path+"')");

}
