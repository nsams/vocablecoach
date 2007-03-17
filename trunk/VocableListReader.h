#ifndef VOCABLELISTREADER_H
#define VOCABLELISTREADER_H
#include <QString>
class VocableListModel;

class VocableListReader {
public:
    VocableListReader(const QString& filename);
	bool read(VocableListModel* model);
private:
	QString m_fileName;
};

#endif
