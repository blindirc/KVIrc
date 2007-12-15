/****************************************************************************
**
** Copyright (C) 1992-2007 Trolltech ASA. All rights reserved.
**
** This file is part of the Qt Assistant of the Qt Toolkit.
**
** This file may be used under the terms of the GNU General Public
** License version 2.0 as published by the Free Software Foundation
** and appearing in the file LICENSE.GPL included in the packaging of
** this file.  Please review the following information to ensure GNU
** General Public Licensing requirements will be met:
** http://www.trolltech.com/products/qt/opensource.html
**
** If you are unsure which license is appropriate for your use, please
** review the following information:
** http://www.trolltech.com/products/qt/licensing.html or contact the
** sales department at sales@trolltech.com.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#ifndef INDEX_H
#define INDEX_H

#include <QStringList>
#include <QHash>
#include <QDataStream>
#include <QObject>
#include <QList>
#include <QFile>
#include <QVector>

struct Document {
	Document( int d, int f ) : docNumber( d ), frequency( f ) {}
	Document() : docNumber( -1 ), frequency( 0 ) {}
	bool operator==( const Document &doc ) const {
		return docNumber == doc.docNumber;
	}
	bool operator<( const Document &doc ) const {
		return frequency > doc.frequency;
	}
	bool operator<=( const Document &doc ) const {
		return frequency >= doc.frequency;
	}
	bool operator>( const Document &doc ) const {
		return frequency < doc.frequency;
	}
	qint16 docNumber;
	qint16 frequency;
};

QDataStream &operator>>( QDataStream &s, Document &l );
QDataStream &operator<<( QDataStream &s, const Document &l );

class Index : public QObject
{
	Q_OBJECT
public:
	struct Entry {
		Entry( int d ) { documents.append( Document( d, 1 ) ); }
		Entry( QVector<Document> l ) : documents( l ) {}
		QVector<Document> documents;
	};
	struct PosEntry {
		PosEntry( int p ) { positions.append( p ); }
		QList<uint> positions;
	};

	Index( const QString &dp, const QString &hp );
	Index( const QStringList &dl, const QString &hp );
	void writeDict();
	void readDict();
	int makeIndex();
	QStringList query( const QStringList&, const QStringList&, const QStringList& );
	QString getDocumentTitle( const QString& );
	void setDictionaryFile( const QString& );
	void setDocListFile( const QString& );
	void setDocList( const QStringList & );
	void writeDocumentList();
	const QStringList& documentList() { return docList; };
	const QStringList& titlesList() { return titleList; };

signals:
	void indexingProgress( int );

private slots:
	void setLastWinClosed();

private:
	void setupDocumentList();
	void parseDocument( const QString&, int );
	void insertInDict( const QString&, int );
	void readDocumentList();
	QStringList getWildcardTerms( const QString& );
	QStringList split( const QString& );
	QVector<Document> setupDummyTerm( const QStringList& );
	bool searchForPattern( const QStringList&, const QStringList&, const QString& );
	void buildMiniDict( const QString& );
	QString getCharsetForDocument(QFile *);
	QStringList docList;
	QStringList titleList;
	QHash<QString, Entry*> dict;
	QHash<QString, PosEntry*> miniDict;
	uint wordNum;
	QString docPath;
	QString dictFile, docListFile;
	bool alreadyHaveDocList;
	bool lastWindowClosed;
	QHash<QString, QString> documentTitleCache;
};

#endif
