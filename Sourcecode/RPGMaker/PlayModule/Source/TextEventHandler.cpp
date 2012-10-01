#include "qdebug.h"
#include "qpixmap.h"
#include "qpainter.h"
#include "qtextbrowser.h"

#include "../ProgramDefaults.h"
#include "../Headers/TextEventHandler.h"

using namespace playmodule;

TextEventHandler::TextEventHandler(QGraphicsItem *parent)
	: QGraphicsPixmapItem(parent) {

		createWindow();

		_textField = new QGraphicsTextItem(this);
		_textField->setTextWidth(boundingRect().width()-50);
		_textField->setDefaultTextColor(QColor(Qt::white));
		_textField->setPos(25, 10);
}

TextEventHandler::~TextEventHandler() {}

int TextEventHandler::getIndex() const {
	return _index;
}

TextEvent* TextEventHandler::getEvent() const {
	return _event;
}

void TextEventHandler::setEvent(TextEvent *event) {
	_event = event;
	_index = 0;
}

void TextEventHandler::nextMessage() {
	QVector<QString> messages = _event->getMessages();
	if (_index < messages.size()) {

		_textField->setPlainText(messages.at(_index));

		QFont font("Times", 12, QFont::Bold);
		_textField->setFont(font);

		QTextBlockFormat format;
		format.setAlignment(Qt::AlignLeft);
		QTextCursor cursor = _textField->textCursor();
		cursor.select(QTextCursor::Document);
		cursor.mergeBlockFormat(format);
		cursor.clearSelection();
		_textField->setTextCursor(cursor);

		_index++;
	} else {
		QGraphicsPixmapItem::hide();
		emit eventFinishedSignal();
	}
}

void TextEventHandler::keyPressEvent(QKeyEvent *event) {
	if ((event->key() == Qt::Key_Enter) || (event->key() == Qt::Key_Return))
		nextMessage();
}

void TextEventHandler::createWindow() {
	// Get and resize the textballoon
	QPixmap windowSkin(QPixmap(g_windowskinPath + "001-Blue01.png"));
	QPixmap textBalloon(windowSkin.copy(0, 0, 128, 128));
	textBalloon = textBalloon.scaled(QSize(500, 200));

	// Paint the textballoon's border
	QPainter painter(&textBalloon);
	painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
	QPixmap borderTile(16, 16);

	// Northern border
	borderTile = windowSkin.copy(152, 0, 16, 16);
	for (int i = 1; i < textBalloon.width()/16; i++)
		painter.drawPixmap(borderTile.width() * i, 0, borderTile);

	// Southern border
	borderTile = windowSkin.copy(152, 48, 16, 16);
	for (int i = 1; i < textBalloon.width()/16; i++)
		painter.drawPixmap(borderTile.width() * i, textBalloon.height() - borderTile.height(), borderTile);

	// Western border
	borderTile = windowSkin.copy(128, 24, 16, 16);
	for (int i = 1; i < textBalloon.height()/16; i++)
		painter.drawPixmap(0, borderTile.height() * i, borderTile);

	// Eastern border
	borderTile = windowSkin.copy(176, 24, 16, 16);
	for (int i = 1; i < textBalloon.height()/16; i++)
		painter.drawPixmap(textBalloon.width() - borderTile.width(), borderTile.height() * i, borderTile);

	// North-West corner
	borderTile = windowSkin.copy(128, 0, 16, 16);
	painter.drawPixmap(0, 0, borderTile);

	// North-East corner
	borderTile = windowSkin.copy(176, 0, 16, 16);
	painter.drawPixmap(textBalloon.width()-16, 0, borderTile);

	// South-West corner
	borderTile = windowSkin.copy(128, 48, 16, 16);
	painter.drawPixmap(0, textBalloon.height()-16, borderTile);

	// South-East corner
	borderTile = windowSkin.copy(176, 48, 16, 16);
	painter.drawPixmap(textBalloon.width()-16, textBalloon.height()-16, borderTile);

	setPixmap(textBalloon);
}