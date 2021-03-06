#pragma once

#include <QMultiMap>
#include <QMapIterator>

template<class T>
class TopNSearch {
public:
	static enum { ORDER_ASC = 0, ORDER_DESC };

private:
	QMultiMap<qreal, T> data;

public:
	TopNSearch() {}
	~TopNSearch() {}

	void add(qreal key, T value) {
		data.insert(key, value);
	}

	QList<T> topN(int n, int orderType) {
		QList<T> ret;

		int count = 0;

		if (orderType == ORDER_ASC) {
			for (QMultiMap<qreal, T>::iterator it = data.begin(); it != data.end() && count < n; ++it) {
				QList<T> values = data.values(it.key());
				for (int i = 0; i < values.size() && count < n; i++, count++) {
					ret.push_back(values[i]);
				}
			}
		} else {
			QMapIterator<qreal, T> it(data);
			//QMultiMap<qreal, T>::iterator it = data.begin();
			it.toBack();
			while (it.hasPrevious() && count < n) {
				it.previous();
				//QList<T> values = data.values(it.key());
				T value = it.value();
				ret.push_back(value);
				count++;
				/*
				for (int i = 0; i < values.size() && count < n; i++, count++) {
					ret.push_back(values[i]);
				}
				*/
			}
		}

		return ret;
	}
};

