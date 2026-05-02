#include <QApplication>
#include <QWidget>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    QWidget w;
    w.resize(400, 300);
    w.setWindowTitle("Qt работает!");
    w.show();
    return app.exec();
}