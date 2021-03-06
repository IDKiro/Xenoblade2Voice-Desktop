#include <QDialog>
#include <QFileDialog>
#include <QLabel>
#include <QDebug>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonArray>
#include "QDesktopServices"
#include "algorithm"
#include "iostream"
#include "string"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "cglobal.h"

QJsonObject textObj;


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->setFixedSize(this->geometry().width(),this->geometry().height());
	this->setWindowFlags(Qt::Window | Qt::FramelessWindowHint | Qt::WindowMinimizeButtonHint);

	initSetting();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initSetting()
{
	QString fileName;
	QString staff;
	if (ui->radioButton_lang->isChecked())
	{
		if (ui->radioButton_dlc->isChecked())
		{
			fileName = "assets/icon/titlelogo_cn.png";
		}
		else
		{
			fileName = "assets/icon/titlelogo_cn_DLC.png";
		}
		ui->radioButton_lang->setText(u8"中文");
		if (ui->radioButton_dlc->isChecked())
		{
			ui->radioButton_dlc->setText(u8"本篇");
		}
		else
		{
			ui->radioButton_dlc->setText(u8"黄金国");
		}
		staff = u8"<html><head/><body><p>数据挖掘:</p><p>	@Moosehunter(Xenoblade Subreddit Discord)</p><p>对话听写与翻译:</p><p>	@vinsanityvc(NGA)</p><p>	@takamana(stage1st)</p><p>游戏内对话确认协助:</p><p>	@天天の幻想(Baidu Tieba: 异度之刃)</p><p>	@nanami_c(Baidu Tieba: 异度之刃)</p><p>	@黑礼帽的誓言(Baidu Tieba: 异度之刃)</p><p>主程序:</p><p>	@IDKiro(NS in ZJU QQ Group)</p><p>特别感谢:</p><p>	@TheKjozk(YouTube)</p><p>	Monolith Soft &amp; Nintendo</p></body></html>";
		ui->graphicsView_title->setToolTip(staff);
		//qDebug() << "Load CN title";
	}
	else
	{
		if (ui->radioButton_dlc->isChecked())
		{
			fileName = "assets/icon/titlelogo_jp.png";
		}
		else
		{
			fileName = "assets/icon/titlelogo_jp_DLC.png";
		}
		ui->radioButton_lang->setText(u8"日本語");
		if (ui->radioButton_dlc->isChecked())
		{
			ui->radioButton_dlc->setText(u8"本編");
		}
		else
		{
			ui->radioButton_dlc->setText(u8"黄金の国");
		}
		staff = u8"<html><head/><body><p>データマイニング:</p><p>	@Moosehunter(Xenoblade Subreddit Discord)</p><p>会話聞き取りと中国語翻訳:</p><p>	@vinsanityvc(NGA)</p><p>	@takamana(stage1st)</p><p>ゲーム内で会話確認の協力:</p><p>	@天天の幻想(Baidu Tieba: 异度之刃)</p><p>	@nanami_c(Baidu Tieba: 异度之刃)</p><p>	@黑礼帽的誓言(Baidu Tieba: 异度之刃)</p><p>プログラマー:</p><p>	@IDKiro(NS in ZJU QQ Group)</p><p>スペシャルサンクス:</p><p>	@TheKjozk(YouTube)</p><p>	Monolith Soft &amp; Nintendo</p></body></html>";
		ui->graphicsView_title->setToolTip(staff);
		//qDebug() << "Load JP title";
	}

	QImage* img = new QImage;
	if (!(img->load(fileName)))
	{
		//qDebug() << "Load failed";
		delete img;
		return;
	}
	QGraphicsScene *scene = new QGraphicsScene;
	scene->addPixmap(QPixmap::fromImage(*img));
	ui->graphicsView_title->setScene(scene);

	ui->listWidget_char->clear();

	if (CGlobal::curid_1 != "0" && ui->graphicsView_char_1->scene())
	{
		ui->graphicsView_char_1->scene()->clear();
		CGlobal::curid_1 = "0";
	}
	if (CGlobal::curid_2 != "0" && ui->graphicsView_char_2->scene())
	{
		ui->graphicsView_char_2->scene()->clear();
		CGlobal::curid_2 = "0";
	}
	if (CGlobal::curid_3 != "0" && ui->graphicsView_char_3->scene())
	{
		ui->graphicsView_char_3->scene()->clear();
		CGlobal::curid_3 = "0";
	}
	CGlobal::flag = 0;

	//解析json
	QFile loadFile;
	if (ui->radioButton_dlc->isChecked())
	{
		loadFile.setFileName("assets/json/jsonChara.json");
	}
	else
	{
		loadFile.setFileName("assets/json/jsonChara_DLC.json");
	}
	
	if (!loadFile.open(QIODevice::ReadOnly))
	{
		//qDebug() << "could't open projects json";
		return;
	}

	QByteArray allData = loadFile.readAll();
	loadFile.close();

	QJsonParseError json_error;
	QJsonDocument jsonDoc(QJsonDocument::fromJson(allData, &json_error));

	if (json_error.error != QJsonParseError::NoError)
	{
		//qDebug() << "json error!";
		return;
	}

	QJsonObject rootObj = jsonDoc.object();
	QStringList keys = rootObj.keys();
	CGlobal::CharNum = keys.size();

	//获取角色的名称和头像
	for (int i = 0; i < keys.size(); i++)
	{
		if (rootObj.contains(keys.at(i)))
		{
			//qDebug() << keys.at(i);
			fileName = "assets/image/" + keys.at(i) + ".png";									//fileName已经定义过，直接使用
																								//qDebug() << fileName;
			QJsonObject subObj = rootObj.value(keys.at(i)).toObject();
			if (subObj.contains("name"))
			{
				QJsonObject sub2Obj = subObj.value("name").toObject();
				QPixmap pix(fileName);

				if (ui->radioButton_lang->isChecked())
				{
					QListWidgetItem *item = new QListWidgetItem(QIcon(pix), sub2Obj["CN"].toString());
					item->setToolTip(sub2Obj["JP"].toString());
					item->setStatusTip("CN");
					item->setWhatsThis(keys.at(i));
					ui->listWidget_char->addItem(item);
				}
				else
				{
					QListWidgetItem *item = new QListWidgetItem(QIcon(pix), sub2Obj["JP"].toString());
					item->setToolTip(sub2Obj["CN"].toString());
					item->setStatusTip("JP");
					item->setWhatsThis(keys.at(i));
					ui->listWidget_char->addItem(item);
				}
				//qDebug() << "name_CN is:" << sub2Obj["CN"].toString();
				//qDebug() << "name_JP is:" << sub2Obj["JP"].toString();
			}
		}
	}

	resetTalklist();

	//解析结束

	connect(updater, SIGNAL(timeout()), this, SLOT(update()));

	player->setVolume(100);

	ui->listWidget->setCurrentRow(0);


	if (ui->listWidget->count() != 0) {
		loadTrack();
		player->pause();
		updater->start();
	}
}

void MainWindow::on_pushButton_close_clicked()
{
	close();
}

void MainWindow::on_pushButton_min_clicked()
{
	showMinimized();
}

void MainWindow::on_repeat_clicked()
{
	QIcon icon;

	switch (repeat)
	{
	case 0:				//列表播放->列表循环
	{
		icon.addFile(tr("assets/icon/repeat.png"));
		
		repeat = 1;
		break;
	}
	case 1:				//列表循环->单曲循环
	{
		icon.addFile(tr("assets/icon/repeat_one.png"));
		repeat = 2;
		break;
	}
	case 2:				//单曲循环->列表播放
	{
		icon.addFile(tr("assets/icon/reorder.png"));
		repeat = 0;
		break;
	}
	default:
	{
		break;
	}
	}
	ui->repeat->setIcon(icon);
}

void MainWindow::on_listWidget_doubleClicked()
{
    lCounter = getIndex();

    ui->play->setChecked(false);

    loadTrack();
    player->play();

	QIcon icon;
	icon.addFile(tr("assets/icon/pause.png"));
	ui->play->setIcon(icon);

}

void MainWindow::on_play_clicked()
{
	QIcon icon;
    if(ui->listWidget->count() != 0)
    if(player->state() == QMediaPlayer::PlayingState)
    {
        player->pause();
		icon.addFile(tr("assets/icon/play.png"));
		ui->play->setIcon(icon);
    }
   else
   {
        player->play();
        updater->start();
		icon.addFile(tr("assets/icon/pause.png"));
		ui->play->setIcon(icon);
   }
}

void MainWindow::on_forward_clicked()
{   
	if(ui->listWidget->count() != 0)
    if(repeat == 2)
    {
        repeat = 0;
		next();
		repeat = 2;
    }
    else
    {
        next();
    }
}

void MainWindow::on_back_clicked()
{   
	if(ui->listWidget->count() != 0)
    if(player->position() > 3000)
    {
       player->setPosition(0);
    }
    else
    {
        back();
    }
}

void MainWindow::on_volumeBar_valueChanged(int value)
{
    player->setVolume(value);
    (value != 0)?ui->mute->setChecked(false):ui->mute->setChecked(true);
}

void MainWindow::on_seekBar_sliderMoved(int position)
{
    player->setPosition(player->duration() / 1000 * position);
}

void MainWindow::on_mute_clicked()
{
	QIcon icon;
    muted = !muted;
    (muted)?player->setVolume(0):player->setVolume(ui->volumeBar->value());
	if (muted)
	{
		icon.addFile(tr("assets/icon/mute.png"));
		ui->mute->setIcon(icon);
	}
	else
	{
		icon.addFile(tr("assets/icon/volume.png"));
		ui->mute->setIcon(icon);
	}
}

void MainWindow::update()
{   
	if(!ui->seekBar->isSliderDown())
        ui->seekBar->setValue((double)player->position()/player->duration() * 1000);

    if(player->state() == QMediaPlayer::StoppedState)
    {
        next();
    }
}

void MainWindow::updateList()
{
    ui->listWidget->clear();
    ui->listWidget->addItems(playlist.getTracksNameList());
}

int MainWindow::getIndex()
{
    return ui->listWidget->currentIndex().row();
}

void MainWindow::resetTalklist()
{
	//重置对话组列表
	ui->listWidget_talk->clear();
	//读取json2
	QFile loadFile2;
	if (ui->radioButton_dlc->isChecked())
	{
		loadFile2.setFileName("assets/json/jsonDialogue.json");
	}
	else
	{
		loadFile2.setFileName("assets/json/jsonDialogue_DLC.json");
	}
	
	if (!loadFile2.open(QIODevice::ReadOnly))
	{
		//qDebug() << "could't open projects json";
		return;
	}

	QByteArray allData2 = loadFile2.readAll();
	loadFile2.close();

	QJsonParseError json_error2;
	QJsonDocument jsonDoc2(QJsonDocument::fromJson(allData2, &json_error2));

	if (json_error2.error != QJsonParseError::NoError)
	{
		//qDebug() << "json error!";
		return;
	}
	QJsonObject rootObj2 = jsonDoc2.object();
	QStringList keys2 = rootObj2.keys();

	//加入所有对话组
	for (int i = 0; i < keys2.size(); i++)
	{
		if (rootObj2.contains(keys2.at(i)))
		{
			QJsonObject subObj2 = rootObj2.value(keys2.at(i)).toObject();	//读取对话组的名称
			if (subObj2.contains("name"))
			{
				QJsonObject sub2Obj2 = subObj2.value("name").toObject();	//读取对话组的名称

				if (ui->radioButton_lang->isChecked())
				{
					QListWidgetItem *item = new QListWidgetItem(sub2Obj2["CN"].toString());
					item->setToolTip(sub2Obj2["JP"].toString());
					item->setStatusTip("CN");
					item->setWhatsThis(keys2.at(i));
					ui->listWidget_talk->addItem(item);
				}
				else
				{
					QListWidgetItem *item = new QListWidgetItem(sub2Obj2["JP"].toString());
					item->setToolTip(sub2Obj2["CN"].toString());
					item->setStatusTip("JP");
					item->setWhatsThis(keys2.at(i));
					ui->listWidget_talk->addItem(item);
				}

			}
		}
	}
}

void MainWindow::resetSelect()
{
	for (int i = 0; i < CGlobal::CharNum; i++)
	{
		ui->listWidget_char->item(i)->setFlags(ui->listWidget_char->item(i)->flags() | (Qt::ItemIsSelectable)); //设置该项为可用
		ui->listWidget_char->item(i)->setHidden(false);
	}
	
	resetTalklist();
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Return :
    {
        lCounter = getIndex();
        if(lCounter != -1)
        {
            ui->play->setChecked(false);

           loadTrack();
           player->play();
        }
        break;
    }
    case Qt::Key_Up :
    {
        int ind = getIndex() - 1;if(ind < 0)ind = ui->listWidget->count() - 1;
        ui->listWidget->setCurrentRow(ind);
        break;
    }
    case Qt::Key_Down :
    {
        int ind = getIndex() + 1;if(ind >= ui->listWidget->count())ind = 0;
        ui->listWidget->setCurrentRow(ind);
        break;
    }
    default :
    {
        break;
    }
    }
}

void MainWindow::next()
{
    lCounter++;

    if(repeat == 2)
    {
        lCounter--;
    }

	if (lCounter >= ui->listWidget->count())
	{
		lCounter = 0;
	}
        

    ui->listWidget->setCurrentRow(lCounter);

    ui->play->setChecked(false);

    loadTrack();

	if (lCounter == 0 && repeat == 0)
	{
		QIcon icon;

		player->pause();
		icon.addFile(tr("assets/icon/play.png"));
		ui->play->setIcon(icon);
	}
	else
	{
		player->play();
	}
}

void MainWindow::back()
{
     lCounter--;

     if(lCounter < 0)
        lCounter = ui->listWidget->count() - 1;


     ui->listWidget->setCurrentRow(lCounter);

     ui->play->setChecked(false);

     loadTrack();
     player->play();
}

void MainWindow::loadTrack()
{
     QString qstr = QString::fromStdString(playlist.tracks[getIndex()].getLocation());
     player->setMedia(QUrl::fromLocalFile(qstr));
     qstr = QString::fromStdString(playlist.tracks[getIndex()].getName());
}

void MainWindow::selectCN()
{
	QString fileName;

	if (ui->radioButton_dlc->isChecked())
	{
		fileName = "assets/icon/titlelogo_cn.png";
	}
	else
	{
		fileName = "assets/icon/titlelogo_cn_DLC.png";
	}
	QString staff;
	staff = u8"<html><head/><body><p>数据挖掘:</p><p>	@Moosehunter(Xenoblade Subreddit Discord)</p><p>对话听写与翻译:</p><p>	@vinsanityvc(NGA)</p><p>	@takamana(stage1st)</p><p>游戏内对话确认协助:</p><p>	@天天の幻想(Baidu Tieba: 异度之刃)</p><p>	@nanami_c(Baidu Tieba: 异度之刃)</p><p>	@黑礼帽的誓言(Baidu Tieba: 异度之刃)</p><p>主程序:</p><p>	@IDKiro(NS in ZJU QQ Group)</p><p>特别感谢:</p><p>	@TheKjozk(YouTube)</p><p>	Monolith Soft &amp; Nintendo</p></body></html>";
	ui->graphicsView_title->setToolTip(staff);
	//qDebug() << "Load CN title";

	QStringList sub2keys2 = textObj.keys();

	for (int i = 0; i < sub2keys2.size(); i++)
	{
		if (textObj.contains(sub2keys2.at(i)))
		{

			QJsonObject sub3Obj2 = textObj.value(sub2keys2.at(i)).toObject();

			playlist.tracks[i].setName(sub3Obj2["CN"].toString().toStdString());
		}
	}	

	int curRow = ui->listWidget->currentRow();
	updateList();

	for (int i = 0; i < sub2keys2.size(); i++)
	{
		if (textObj.contains(sub2keys2.at(i)))
		{

			QJsonObject sub3Obj2 = textObj.value(sub2keys2.at(i)).toObject();

			ui->listWidget->item(i)->setToolTip(sub3Obj2["JP"].toString());
		}
	}

	ui->listWidget->setCurrentRow(curRow);

	QImage* img = new QImage;
	if (!(img->load(fileName)))
	{
		//qDebug() << "Load failed";
		delete img;
		return;
	}
	QGraphicsScene *scene = new QGraphicsScene;
	scene->addPixmap(QPixmap::fromImage(*img));
	ui->graphicsView_title->setScene(scene);
	
	for (int i = 0; i < ui->listWidget_char->count(); i++)
	{
		if (ui->listWidget_char->item(i)->statusTip() == "JP")
		{
			
			QString textTemp;
			
			textTemp = ui->listWidget_char->item(i)->text();
			ui->listWidget_char->item(i)->setText(ui->listWidget_char->item(i)->toolTip());
			ui->listWidget_char->item(i)->setToolTip(textTemp);

			ui->listWidget_char->item(i)->setStatusTip("CN");
		}
	}

	for (int i = 0; i < ui->listWidget_talk->count(); i++)
	{
		if (ui->listWidget_talk->item(i)->statusTip() == "JP")
		{

			QString textTemp;

			textTemp = ui->listWidget_talk->item(i)->text();
			ui->listWidget_talk->item(i)->setText(ui->listWidget_talk->item(i)->toolTip());
			ui->listWidget_talk->item(i)->setToolTip(textTemp);

			ui->listWidget_talk->item(i)->setStatusTip("CN");
		}
	}
}

void MainWindow::selectJP()
{
	QString fileName;
	if (ui->radioButton_dlc->isChecked())
	{
		fileName = "assets/icon/titlelogo_jp.png";
	}
	else
	{
		fileName = "assets/icon/titlelogo_jp_DLC.png";
	}
	QString staff;
	staff = u8"<html><head/><body><p>データマイニング:</p><p>	@Moosehunter(Xenoblade Subreddit Discord)</p><p>会話聞き取りと中国語翻訳:</p><p>	@vinsanityvc(NGA)</p><p>	@takamana(stage1st)</p><p>ゲーム内で会話確認の協力:</p><p>	@天天の幻想(Baidu Tieba: 异度之刃)</p><p>	@nanami_c(Baidu Tieba: 异度之刃)</p><p>	@黑礼帽的誓言(Baidu Tieba: 异度之刃)</p><p>プログラマー:</p><p>	@IDKiro(NS in ZJU QQ Group)</p><p>スペシャルサンクス:</p><p>	@TheKjozk(YouTube)</p><p>	Monolith Soft &amp; Nintendo</p></body></html>";
	ui->graphicsView_title->setToolTip(staff);
	//qDebug() << "Load JP title";

	QStringList sub2keys2 = textObj.keys();

	for (int i = 0; i < sub2keys2.size(); i++)
	{

		if (textObj.contains(sub2keys2.at(i)))
		{

			QJsonObject sub3Obj2 = textObj.value(sub2keys2.at(i)).toObject();

			playlist.tracks[i].setName(sub3Obj2["JP"].toString().toStdString());
		}
	}

	int curRow = ui->listWidget->currentRow();
	updateList();

	for (int i = 0; i < sub2keys2.size(); i++)
	{
		if (textObj.contains(sub2keys2.at(i)))
		{

			QJsonObject sub3Obj2 = textObj.value(sub2keys2.at(i)).toObject();

			ui->listWidget->item(i)->setToolTip(sub3Obj2["CN"].toString());
		}
	}

	ui->listWidget->setCurrentRow(curRow);

	QImage* img = new QImage;
	if (!(img->load(fileName)))
	{
		//qDebug() << "Load failed";
		delete img;
		return;
	}
	QGraphicsScene *scene = new QGraphicsScene;
	scene->addPixmap(QPixmap::fromImage(*img));
	ui->graphicsView_title->setScene(scene);

	for (int i = 0; i < ui->listWidget_char->count(); i++)
	{
		if (ui->listWidget_char->item(i)->statusTip() == "CN")
		{

			QString textTemp;

			textTemp = ui->listWidget_char->item(i)->text();
			ui->listWidget_char->item(i)->setText(ui->listWidget_char->item(i)->toolTip());
			ui->listWidget_char->item(i)->setToolTip(textTemp);

			ui->listWidget_char->item(i)->setStatusTip("JP");
		}
	}

	for (int i = 0; i < ui->listWidget_talk->count(); i++)
	{
		if (ui->listWidget_talk->item(i)->statusTip() == "CN")
		{

			QString textTemp;

			textTemp = ui->listWidget_talk->item(i)->text();
			ui->listWidget_talk->item(i)->setText(ui->listWidget_talk->item(i)->toolTip());
			ui->listWidget_talk->item(i)->setToolTip(textTemp);

			ui->listWidget_talk->item(i)->setStatusTip("JP");
		}
	}
}

void MainWindow::on_radioButton_lang_clicked()
{
	if (ui->radioButton_lang->isChecked())
	{
		selectCN();
		ui->radioButton_lang->setText(u8"中文");
		if (ui->radioButton_dlc->isChecked())
		{
			ui->radioButton_dlc->setText(u8"本篇");
		}
		else
		{
			ui->radioButton_dlc->setText(u8"黄金国");
		}
	}
	else
	{
		selectJP();
		ui->radioButton_lang->setText(u8"日本語");
		if (ui->radioButton_dlc->isChecked())
		{
			ui->radioButton_dlc->setText(u8"本編");
		}
		else
		{
			ui->radioButton_dlc->setText(u8"黄金の国");
		}
	}
}

void MainWindow::on_radioButton_dlc_clicked()
{	
	if (ui->radioButton_dlc->isChecked())
	{
		if (ui->radioButton_lang->isChecked())
		{
			ui->radioButton_dlc->setText(u8"本篇");
		}
		else
		{
			ui->radioButton_dlc->setText(u8"本編");
		}
	}
	else
	{
		if (ui->radioButton_lang->isChecked())
		{
			ui->radioButton_dlc->setText(u8"黄金国");
		}
		else
		{
			ui->radioButton_dlc->setText(u8"黄金の国");
		}
	}

	initSetting();
}

void MainWindow::char_1_selected()
{
	QString curid;
	//解析json
	//读取json1
	QFile loadFile;
	if (ui->radioButton_dlc->isChecked())
	{
		loadFile.setFileName("assets/json/jsonChara.json");
	}
	else
	{
		loadFile.setFileName("assets/json/jsonChara_DLC.json");
	}
	
	if (!loadFile.open(QIODevice::ReadOnly))
	{
		//qDebug() << "could't open projects json";
		return;
	}

	QByteArray allData = loadFile.readAll();
	loadFile.close();

	QJsonParseError json_error;
	QJsonDocument jsonDoc(QJsonDocument::fromJson(allData, &json_error));

	if (json_error.error != QJsonParseError::NoError)
	{
		//qDebug() << "json error!";
		return;
	}

	QJsonObject rootObj = jsonDoc.object();

	QStringList keys = rootObj.keys();

	//开始处理json数据
	if (rootObj.contains(CGlobal::curid_1))
	{
		QJsonObject subObj = rootObj.value(CGlobal::curid_1).toObject();	//读取当前角色的数据

		//使不可对话的对象不可选择
		if (subObj.contains("disabledChara"))
		{
			QJsonArray sub2Array = subObj.value("disabledChara").toArray();
			//qDebug() << "disabledChara is:";
			for (int j = 0; j < sub2Array.size(); j++)
			{
				//qDebug() << j << " value is:" << sub2Array.at(j).toString();
				for (int k = 0; k < keys.size(); k++)
				{
					curid = ui->listWidget_char->item(k)->whatsThis();
					if (curid == sub2Array.at(j).toString())
					{
						ui->listWidget_char->item(k)->setFlags(ui->listWidget_char->item(k)->flags() & ~(Qt::ItemIsSelectable)); //设置该项为不可用
						ui->listWidget_char->item(k)->setHidden(true);
					}

				}
			}
		}

		//读取json2
		QFile loadFile2;
		if (ui->radioButton_dlc->isChecked())
		{
			loadFile2.setFileName("assets/json/jsonDialogue.json");
		}
		else
		{
			loadFile2.setFileName("assets/json/jsonDialogue_DLC.json");
		}
		
		if (!loadFile2.open(QIODevice::ReadOnly))
		{
			//qDebug() << "could't open projects json";
			return;
		}

		QByteArray allData2 = loadFile2.readAll();
		loadFile2.close();

		QJsonParseError json_error2;
		QJsonDocument jsonDoc2(QJsonDocument::fromJson(allData2, &json_error2));

		if (json_error2.error != QJsonParseError::NoError)
		{
			//qDebug() << "json error!";
			return;
		}

		//删除当前不可进行的对话组
		if (subObj.contains("includedPattern"))
		{
			QJsonArray sub2Array = subObj.value("includedPattern").toArray();		//当前角色所包含的对话，删除不包含的listwidget中的项
			QJsonObject rootObj2 = jsonDoc2.object();

			for (int i = ui->listWidget_talk->count()-1; i >= 0; i--)
			{
				int delflag = 1;
				for (int j = 0; j < sub2Array.size(); j++)
				{
					if (sub2Array.at(j).toString() == ui->listWidget_talk->item(i)->whatsThis())
					{
						delflag = 0;
						break;
					}
				}
				if (delflag)
				{
					ui->listWidget_talk->takeItem(i);	//删除该行
				}
				
			}
		}
	}
	//解析结束
}

void MainWindow::char_2_selected()
{
	QString curid;
	//解析json
	//读取json1
	QFile loadFile;
	if (ui->radioButton_dlc->isChecked())
	{
		loadFile.setFileName("assets/json/jsonChara.json");
	}
	else
	{
		loadFile.setFileName("assets/json/jsonChara_DLC.json");
	}
	
	if (!loadFile.open(QIODevice::ReadOnly))
	{
		//qDebug() << "could't open projects json";
		return;
	}

	QByteArray allData = loadFile.readAll();
	loadFile.close();

	QJsonParseError json_error;
	QJsonDocument jsonDoc(QJsonDocument::fromJson(allData, &json_error));

	if (json_error.error != QJsonParseError::NoError)
	{
		//qDebug() << "json error!";
		return;
	}

	QJsonObject rootObj = jsonDoc.object();

	QStringList keys = rootObj.keys();

	//开始处理json数据
	if (rootObj.contains(CGlobal::curid_2))
	{
		QJsonObject subObj = rootObj.value(CGlobal::curid_2).toObject();	//读取当前角色的数据


		//读取json3
		QFile loadFile3;
		if (ui->radioButton_dlc->isChecked())
		{
			loadFile3.setFileName("assets/json/jsonTwoChara.json");
		}
		else
		{
			loadFile3.setFileName("assets/json/jsonTwoChara_DLC.json");
		}
		
		if (!loadFile3.open(QIODevice::ReadOnly))
		{
			//qDebug() << "could't open projects json";
			return;
		}

		QByteArray allData3 = loadFile3.readAll();
		loadFile3.close();

		QJsonParseError json_error3;
		QJsonDocument jsonDoc3(QJsonDocument::fromJson(allData3, &json_error3));

		if (json_error3.error != QJsonParseError::NoError)
		{
			//qDebug() << "json error!";
			return;
		}

		QJsonObject rootObj3 = jsonDoc3.object();

		QStringList keys3 = rootObj3.keys();


		//使不可对话的对象不可选择
		if (rootObj3.contains(CGlobal::curid_1))
		{
			QJsonObject subObj3 = rootObj3.value(CGlobal::curid_1).toObject();
			if (subObj3.contains(CGlobal::curid_2))
			{
				QJsonArray sub2Array3 = subObj3.value(CGlobal::curid_2).toArray();
				//qDebug() << "disabledChara is:";
				for (int j = 0; j < sub2Array3.size(); j++)
				{
					//qDebug() << j << " value is:" << sub2Array3.at(j).toString();
					for (int k = 0; k < keys.size(); k++)
					{
						curid = ui->listWidget_char->item(k)->whatsThis();
						if (curid == sub2Array3.at(j).toString())
						{
							ui->listWidget_char->item(k)->setFlags(ui->listWidget_char->item(k)->flags() & ~(Qt::ItemIsSelectable)); //设置该项为不可用
							ui->listWidget_char->item(k)->setHidden(true);
						}

					}
				}
			}
		}

		//读取json2
		QFile loadFile2;
		if (ui->radioButton_dlc->isChecked())
		{
			loadFile2.setFileName("assets/json/jsonDialogue.json");
		}
		else
		{
			loadFile2.setFileName("assets/json/jsonDialogue_DLC.json");
		}
		
		if (!loadFile2.open(QIODevice::ReadOnly))
		{
			//qDebug() << "could't open projects json";
			return;
		}

		QByteArray allData2 = loadFile2.readAll();
		loadFile2.close();

		QJsonParseError json_error2;
		QJsonDocument jsonDoc2(QJsonDocument::fromJson(allData2, &json_error2));

		if (json_error2.error != QJsonParseError::NoError)
		{
			//qDebug() << "json error!";
			return;
		}

		//删除当前不可进行的对话组
		if (subObj.contains("includedPattern"))
		{
			QJsonArray sub2Array = subObj.value("includedPattern").toArray();		//当前角色所包含的对话，删除不包含的listwidget中的项
			QJsonObject rootObj2 = jsonDoc2.object();

			for (int i = ui->listWidget_talk->count() - 1; i >= 0; i--)
			{
				int delflag = 1;
				for (int j = 0; j < sub2Array.size(); j++)
				{
					if (sub2Array.at(j).toString() == ui->listWidget_talk->item(i)->whatsThis())
					{
						delflag = 0;
						break;
					}
				}
				if (delflag)
				{
					ui->listWidget_talk->takeItem(i);	//删除该行
				}

			}
		}
	}
	//解析结束
}

void MainWindow::char_3_selected()
{
	QString curid;
	//解析json
	//读取json1
	QFile loadFile;
	if (ui->radioButton_dlc->isChecked())
	{
		loadFile.setFileName("assets/json/jsonChara.json");
	}
	else
	{
		loadFile.setFileName("assets/json/jsonChara_DLC.json");
	}
	
	if (!loadFile.open(QIODevice::ReadOnly))
	{
		//qDebug() << "could't open projects json";
		return;
	}

	QByteArray allData = loadFile.readAll();
	loadFile.close();

	QJsonParseError json_error;
	QJsonDocument jsonDoc(QJsonDocument::fromJson(allData, &json_error));

	if (json_error.error != QJsonParseError::NoError)
	{
		//qDebug() << "json error!";
		return;
	}

	QJsonObject rootObj = jsonDoc.object();

	QStringList keys = rootObj.keys();

	//开始处理json数据
	if (rootObj.contains(CGlobal::curid_3))
	{
		QJsonObject subObj = rootObj.value(CGlobal::curid_3).toObject();	//读取当前角色的数据

		//读取json2
		QFile loadFile2;
		if (ui->radioButton_dlc->isChecked())
		{
			loadFile2.setFileName("assets/json/jsonDialogue.json");
		}
		else
		{
			loadFile2.setFileName("assets/json/jsonDialogue_DLC.json");
		}
		
		if (!loadFile2.open(QIODevice::ReadOnly))
		{
			//qDebug() << "could't open projects json";
			return;
		}

		QByteArray allData2 = loadFile2.readAll();
		loadFile2.close();

		QJsonParseError json_error2;
		QJsonDocument jsonDoc2(QJsonDocument::fromJson(allData2, &json_error2));

		if (json_error2.error != QJsonParseError::NoError)
		{
			//qDebug() << "json error!";
			return;
		}

		//删除当前不可进行的对话组
		if (subObj.contains("includedPattern"))
		{
			QJsonArray sub2Array = subObj.value("includedPattern").toArray();		//当前角色所包含的对话，删除不包含的listwidget中的项
			QJsonObject rootObj2 = jsonDoc2.object();

			for (int i = ui->listWidget_talk->count() - 1; i >= 0; i--)
			{
				int delflag = 1;
				for (int j = 0; j < sub2Array.size(); j++)
				{
					if (sub2Array.at(j).toString() == ui->listWidget_talk->item(i)->whatsThis())
					{
						delflag = 0;
						break;
					}
				}
				if (delflag)
				{
					ui->listWidget_talk->takeItem(i);	//删除该行
				}

			}
		}
	}
	//解析结束
}

void MainWindow::on_listWidget_char_clicked()
{
	int curRow;
	QString fileName;

	switch (CGlobal::flag)
	{
	case 0:
	{
		curRow = ui->listWidget_char->currentRow();
		CGlobal::curid_1 = ui->listWidget_char->item(curRow)->whatsThis();

		fileName = "assets/image/" + CGlobal::curid_1 + ".png";
		QImage* img = new QImage;
		if (!(img->load(fileName)))
		{
			//qDebug() << "Load failed";
			delete img;
			return;
		}
		QGraphicsScene *scene = new QGraphicsScene;
		scene->addPixmap(QPixmap::fromImage(*img).scaled(90, 90));
		ui->graphicsView_char_1->setScene(scene);

		char_1_selected();

		CGlobal::flag = 1;

		break;
	}

	case 1:
	{
		curRow = ui->listWidget_char->currentRow();

		if (!(ui->listWidget_char->item(curRow)->flags() & (Qt::ItemIsSelectable)))
		{
			//qDebug() << "Can not select";
			return;
		}

		CGlobal::curid_2 = ui->listWidget_char->item(curRow)->whatsThis();

		fileName = "assets/image/" + CGlobal::curid_2 + ".png";
		QImage* img = new QImage;
		if (!(img->load(fileName)))
		{
			//qDebug() << "Load failed";
			delete img;
			return;
		}
		QGraphicsScene *scene = new QGraphicsScene;
		scene->addPixmap(QPixmap::fromImage(*img).scaled(90, 90));
		ui->graphicsView_char_2->setScene(scene);

		char_2_selected();


		CGlobal::flag = 2;

		break;
	}

	case 2:
	{
		curRow = ui->listWidget_char->currentRow();

		if (!(ui->listWidget_char->item(curRow)->flags() & (Qt::ItemIsSelectable)))
		{
			//qDebug() << "Can not select";
			return;
		}

		CGlobal::curid_3 = ui->listWidget_char->item(curRow)->whatsThis();

		fileName = "assets/image/" + CGlobal::curid_3 + ".png";
		QImage* img = new QImage;
		if (!(img->load(fileName)))
		{
			//qDebug() << "Load failed";
			delete img;
			return;
		}
		QGraphicsScene *scene = new QGraphicsScene;
		scene->addPixmap(QPixmap::fromImage(*img).scaled(90, 90));
		ui->graphicsView_char_3->setScene(scene);
		
		char_3_selected();


		CGlobal::flag = 2;

		break;
	}

	default:
	{
		break;
	}
			
	}
	
}

void MainWindow::on_listWidget_talk_clicked()
{
	int curRow;
	curRow = ui->listWidget_talk->currentRow();

	QString curTalkid;
	curTalkid = ui->listWidget_talk->item(curRow)->whatsThis();

	//读取json2
	QFile loadFile2;
	if (ui->radioButton_dlc->isChecked())
	{
		loadFile2.setFileName("assets/json/jsonDialogue.json");
	}
	else
	{
		loadFile2.setFileName("assets/json/jsonDialogue_DLC.json");
	}
	
	if (!loadFile2.open(QIODevice::ReadOnly))
	{
		//qDebug() << "could't open projects json";
		return;
	}

	QByteArray allData2 = loadFile2.readAll();
	loadFile2.close();

	QJsonParseError json_error2;
	QJsonDocument jsonDoc2(QJsonDocument::fromJson(allData2, &json_error2));

	if (json_error2.error != QJsonParseError::NoError)
	{
		//qDebug() << "json error!";
		return;
	}

	QJsonObject rootObj2 = jsonDoc2.object();

	//读取选取的对话组的所有台词
	if (rootObj2.contains(curTalkid))
	{
		//qDebug() << "Talk ID is: " << curTalkid;
		QJsonObject subObj2 = rootObj2.value(curTalkid).toObject();
		if (subObj2.contains("text"))
		{
			//qDebug() << "text exit";
			QJsonObject sub2Obj2 = subObj2.value("text").toObject();
			textObj = sub2Obj2;
			QStringList sub2keys2 = sub2Obj2.keys();

			ui->listWidget->clear();

			QStringList files;
			bool startUpdater = false;
			
			if (ui->listWidget->count() == 0)
			{
				//qDebug() << "startup";
				startUpdater = true;
			}
			

			for (int i = 0; i < sub2keys2.size(); i++)
			{
				if (sub2Obj2.contains(sub2keys2.at(i)))
				{
					//qDebug() << "textId is" << sub2keys2.at(i);
					QJsonObject sub3Obj2 = sub2Obj2.value(sub2keys2.at(i)).toObject();

					QString fileName = "assets/music/" + sub2keys2.at(i) + ".mp3";
					files.append(fileName);
				}
			}

			if (!files.empty())
			{
				playlist.clear();
				playlist.add(files);
				if (startUpdater) updater->start();
			}

			for (int i = 0; i < sub2keys2.size(); i++)
			{
				if (sub2Obj2.contains(sub2keys2.at(i)))
				{

					QJsonObject sub3Obj2 = sub2Obj2.value(sub2keys2.at(i)).toObject();

					string talkText;

					if (ui->radioButton_lang->isChecked())
					{
						talkText = sub3Obj2["CN"].toString().toStdString();
					}
					else
					{
						talkText = sub3Obj2["JP"].toString().toStdString();
					}
					
					playlist.tracks[i].setName(talkText);
				}
			}

			updateList();

			for (int i = 0; i < sub2keys2.size(); i++)
			{
				if (textObj.contains(sub2keys2.at(i)))
				{

					QJsonObject sub3Obj2 = textObj.value(sub2keys2.at(i)).toObject();

					if (ui->radioButton_lang->isChecked())
					{
						ui->listWidget->item(i)->setToolTip(sub3Obj2["JP"].toString());
					}
					else
					{
						ui->listWidget->item(i)->setToolTip(sub3Obj2["CN"].toString());
					}
					
				}
			}			
		}
	}

	
	//音频加载结束

	lCounter = 0;
	ui->listWidget->setCurrentRow(lCounter);

	ui->play->setChecked(false);

	loadTrack();
	player->play();


	QIcon icon;
	icon.addFile(tr("assets/icon/pause.png"));
	ui->play->setIcon(icon);
}

void MainWindow::on_pushButton_char_1_clicked()
{
	if (CGlobal::curid_1 != "0" && ui->graphicsView_char_1->scene())
	{
		if (CGlobal::curid_2 != "0" && ui->graphicsView_char_2->scene())
		{
			CGlobal::curid_1 = CGlobal::curid_2;

			QImage* img = new QImage;
			if (!(img->load("assets/image/" + CGlobal::curid_1 + ".png")))
			{
				//qDebug() << "Load failed";
				delete img;
				return;
			}
			QGraphicsScene *scene = new QGraphicsScene;
			scene->addPixmap(QPixmap::fromImage(*img).scaled(90, 90));
			ui->graphicsView_char_1->setScene(scene);

			if (CGlobal::curid_3 != "0" && ui->graphicsView_char_3->scene())
			{
				CGlobal::curid_2 = CGlobal::curid_3;

				QImage* img = new QImage;
				if (!(img->load("assets/image/" + CGlobal::curid_2 + ".png")))
				{
					//qDebug() << "Load failed";
					delete img;
					return;
				}
				QGraphicsScene *scene = new QGraphicsScene;
				scene->addPixmap(QPixmap::fromImage(*img).scaled(90, 90));
				ui->graphicsView_char_2->setScene(scene);

				CGlobal::curid_3 = "0";
				ui->graphicsView_char_3->scene()->clear();

				CGlobal::flag = 2;
			}
			else
			{
				CGlobal::curid_2 = "0";
				ui->graphicsView_char_2->scene()->clear();

				CGlobal::flag = 1;
			}
		}
		else
		{
			CGlobal::curid_1 = "0";
			ui->graphicsView_char_1->scene()->clear();

			CGlobal::flag = 0;
		}
	}

	resetSelect();

	switch (CGlobal::flag)
	{
	case 1:
	{
		char_1_selected();

		break;
	}

	case 2:
	{
		char_1_selected();
		char_2_selected();

		break;
	}

	default:
	{
		break;
	}

	}
}

void MainWindow::on_pushButton_char_2_clicked()
{
	if (CGlobal::curid_2 != "0" && ui->graphicsView_char_2->scene())
	{
		if (CGlobal::curid_3 != "0" && ui->graphicsView_char_3->scene())
		{
			CGlobal::curid_2 = CGlobal::curid_3;

			QImage* img = new QImage;
			if (!(img->load("assets/image/" + CGlobal::curid_2 + ".png")))
			{
				//qDebug() << "Load failed";
				delete img;
				return;
			}
			QGraphicsScene *scene = new QGraphicsScene;
			scene->addPixmap(QPixmap::fromImage(*img).scaled(90, 90));
			ui->graphicsView_char_2->setScene(scene);

			CGlobal::curid_3 = "0";
			ui->graphicsView_char_3->scene()->clear();
			CGlobal::flag = 2;
		}
		else
		{
			CGlobal::curid_2 = "0";
			ui->graphicsView_char_2->scene()->clear();

			CGlobal::flag = 1;
		}
	}

	resetSelect();

	switch (CGlobal::flag)
	{
	case 1:
	{
		char_1_selected();

		break;
	}

	case 2:
	{
		char_1_selected();
		char_2_selected();

		break;
	}

	default:
	{
		break;
	}

	}
}

void MainWindow::on_pushButton_char_3_clicked()
{
	if (CGlobal::curid_3 != "0" && ui->graphicsView_char_1->scene())
	{
		CGlobal::curid_3 = "0";
		ui->graphicsView_char_3->scene()->clear();
		CGlobal::flag = 2;
	}

	resetSelect();

	switch (CGlobal::flag)
	{
	case 2:
	{
		char_1_selected();
		char_2_selected();

		break;
	}

	default:
	{
		break;
	}

	}
}