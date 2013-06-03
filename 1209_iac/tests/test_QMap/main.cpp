#include <QtCore>

int main(){
	

	QMap<int, QString> map;
	map.insert(1, "one");
	map.insert(5, "five");
	map.insert(10, "ten");

	map.upperBound(0);      // returns iterator to (1, "one")
	map.upperBound(1);      // returns iterator to (5, "five")
	map.upperBound(2);      // returns iterator to (5, "five")
	map.upperBound(10);     // returns end()
	map.upperBound(999);    // returns end()

	qDebug() << (--map.upperBound(10)).key() << (--map.upperBound(10)).value();

	return 0;
}