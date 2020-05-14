#include "dequalizerdialog.h"

#include <QPainter>
#include <QProxyStyle>
#include <DLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <DSlider>
#include <DBlurEffectWidget>
#include <DTabWidget>
#include <DSwitchButton>
#include <DComboBox>
#include <DTitlebar>
#include <DFrame>
#include <DPushButton>
#include <QDebug>
#include <QMap>

#include <vlc/Audio.h>
#include <vlc/Equalizer.h>
#include <vlc/MediaPlayer.h>


class CustomTabStyle : public QProxyStyle
{
public:
    QSize sizeFromContents(ContentsType type, const QStyleOption *option,
                           const QSize &size, const QWidget *widget) const
    {
        QSize s = QProxyStyle::sizeFromContents(type, option, size, widget);
        if (type == QStyle::CT_TabBarTab) {
            s.transpose();
            s.rwidth() = 144; // 设置每个tabBar中item的大小
            s.rheight() = 30;
        }
        return s;
    }

    void drawControl(ControlElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget) const
    {
        if (element == CE_TabBarTabLabel) {
            if (const QStyleOptionTab *tab = qstyleoption_cast<const QStyleOptionTab *>(option)) {

                if (tab->state & QStyle::State_Selected) {
                    painter->save();
                    painter->setPen(0x89cfff);
                    painter->setBrush(QBrush(0x89cfff));
                    painter->restore();
                }
                QTextOption option;
                option.setAlignment(Qt::AlignCenter);
                if (tab->state & QStyle::State_Selected) {
                    painter->setPen(0xf8fcff);
                } else {
                    painter->setPen(0x5d5d5d);
                }

                painter->drawText(tab->rect, tab->text, option);
                return;
            }
        }

        if (element == CE_TabBarTab) {
            QProxyStyle::drawControl(element, option, painter, widget);
        }
    }
};

DWIDGET_USE_NAMESPACE
class DequalizerDialogPrivate
{
public:
    DequalizerDialogPrivate(DequalizerDialog *parent) : q_ptr(parent) {}

    void initUI(bool flag);

    DTitlebar   *mtitlebar            = nullptr;
    DLabel      *mtitleLabel          = nullptr;
    DLabel      *mswitchLabel         = nullptr;
    DSwitchButton *mswitchBtn         = nullptr;
    DComboBox      *mcombox           = nullptr;
    DPushButton    *saveBtn           = nullptr;
    DSlider     *slider_pre           = nullptr;
    DSlider     *slider_60            = nullptr;
    DSlider     *slider_170           = nullptr;
    DSlider     *slider_310           = nullptr;
    DSlider     *slider_600           = nullptr;
    DSlider     *slider_1k            = nullptr;
    DSlider     *slider_3k            = nullptr;
    DSlider     *slider_6k            = nullptr;
    DSlider     *slider_12k           = nullptr;
    DSlider     *slider_14k           = nullptr;
    DSlider     *slider_16k           = nullptr;

    QStringList    effect_type        = {"自定义", "单调", "古典", "俱乐部", "舞曲", "全低音",
                                         "全低音和高音", "全高音", "耳机", "大厅", "实况", "聚会",
                                         "流行", "雷盖", "摇滚", "斯卡", "柔和", "慢摇", "电子乐"
                                        };

    QMap<DSlider *, int> mapSliders;
    MusicSettings   *settings         = nullptr;

    //读取配置文件
    bool    switch_flag               = false;
    int     curEffect                 = 0;


    VlcMediaPlayer *_mediaPlayer;
    VlcEqualizer *_vlcEqualizer;
    //slidergroup
    DBlurEffectWidget *slWidget  = nullptr;
    DequalizerDialog *q_ptr;

    Q_DECLARE_PUBLIC(DequalizerDialog)
};

void DequalizerDialogPrivate::initUI(bool flag)
{
    Q_Q(DequalizerDialog);

    q->resize(720, 463);

    QFont font;
    font.setFamily("SourceHanSansSC");
    font.setWeight(QFont::Normal);
    font.setPixelSize(17);
    mtitleLabel  = new DLabel("均衡器");
    mtitleLabel->resize(51, 25);
    mtitleLabel->setFont(font);

    mswitchLabel = new DLabel("关");
    mswitchLabel->resize(14, 20);

    mswitchBtn = new DSwitchButton(q);
    mswitchBtn->resize(50, 20);
//    //设置底色
//    DPalette sw;
//    sw.setColor(DPalette::Button,QColor("#000000"));
//    mswitchBtn->setPalette(sw);

    mcombox = new DComboBox(q);
    mcombox->setMinimumSize(240, 36);
    mcombox->addItems(effect_type);


    saveBtn = new DPushButton(q);
    saveBtn->setMinimumSize(110, 36);
    saveBtn->setText("保存");

    slider_pre = new DSlider(Qt::Vertical);
    slider_pre->setValue(0);
    slider_pre->setMinimum(-20);
    slider_pre->setMaximum(20);
    slider_pre->setPageStep(1);
    auto lb_preamplifier = new DLabel("前置放大");

    slider_60 = new DSlider(Qt::Vertical);
    slider_60->setValue(0);
    slider_60->setMinimum(-20);
    slider_60->setMaximum(20);
    slider_60->setPageStep(1);
    auto lb1     = new DLabel("60");

    slider_170 = new DSlider(Qt::Vertical);
    slider_170->setValue(0);
    slider_170->setMinimum(-20);
    slider_170->setMaximum(20);
    slider_170->setPageStep(1);
    auto lb2     = new DLabel("170");

    slider_310 = new DSlider(Qt::Vertical);
    slider_310->setValue(0);
    slider_310->setMinimum(-20);
    slider_310->setMaximum(20);
    slider_310->setPageStep(1);
    auto lb3     = new DLabel("310");

    slider_600 = new DSlider(Qt::Vertical);
    slider_600->setValue(0);
    slider_600->setMinimum(-20);
    slider_600->setMaximum(20);
    slider_600->setPageStep(1);
    auto lb4     = new DLabel("600");

    slider_1k = new DSlider(Qt::Vertical);
    slider_1k->setValue(0);
    slider_1k->setMinimum(-20);
    slider_1k->setMaximum(20);
    slider_1k->setPageStep(1);
    auto lb5     = new DLabel("1K");

    slider_3k = new DSlider(Qt::Vertical);
    slider_3k->setValue(0);
    slider_3k->setMinimum(-20);
    slider_3k->setMaximum(20);
    slider_3k->setPageStep(1);

    auto lb6     = new DLabel("3K");

    slider_6k = new DSlider(Qt::Vertical);
    slider_6k->setValue(0);
    slider_6k->setMinimum(-20);
    slider_6k->setMaximum(20);
    slider_6k->setPageStep(1);
    auto lb7    = new DLabel("6K");

    slider_12k = new DSlider(Qt::Vertical);
    slider_12k->setValue(0);
    slider_12k->setMinimum(-20);
    slider_12k->setMaximum(20);
    slider_12k->setPageStep(1);
    auto lb8     = new DLabel("12K");

    slider_14k = new DSlider(Qt::Vertical);
    slider_14k->setValue(0);
    slider_14k->setMinimum(-20);
    slider_14k->setMaximum(20);
    slider_14k->setPageStep(1);
    auto lb9     = new DLabel("14K");

    slider_16k = new DSlider(Qt::Vertical);
    slider_16k->setValue(0);
    slider_16k->setMinimum(-20);
    slider_16k->setMaximum(20);
    slider_16k->setPageStep(1);
    auto lb10     = new DLabel("16K");

    mapSliders.insert(slider_pre, 0);
    mapSliders.insert(slider_60, 1);
    mapSliders.insert(slider_170, 2);
    mapSliders.insert(slider_310, 3);
    mapSliders.insert(slider_600, 4);
    mapSliders.insert(slider_1k, 5);
    mapSliders.insert(slider_3k, 6);
    mapSliders.insert(slider_6k, 7);
    mapSliders.insert(slider_12k, 8);
    mapSliders.insert(slider_14k, 9);
    mapSliders.insert(slider_16k, 10);

    auto vlayout0  = new QVBoxLayout;
    auto vlayout1  = new QVBoxLayout;
    auto vlayout2  = new QVBoxLayout;
    auto vlayout3  = new QVBoxLayout;
    auto vlayout4  = new QVBoxLayout;
    auto vlayout5  = new QVBoxLayout;
    auto vlayout6  = new QVBoxLayout;
    auto vlayout7  = new QVBoxLayout;
    auto vlayout8  = new QVBoxLayout;
    auto vlayout9  = new QVBoxLayout;
    auto vlayout10 = new QVBoxLayout;

    vlayout0->addWidget(slider_pre, Qt::AlignCenter);
    vlayout0->addWidget(lb_preamplifier);

    vlayout1->addWidget(slider_60);
    vlayout1->addWidget(lb1);

    vlayout2->addWidget(slider_170);
    vlayout2->addWidget(lb2);

    vlayout3->addWidget(slider_310);
    vlayout3->addWidget(lb3);

    vlayout4->addWidget(slider_600);
    vlayout4->addWidget(lb4);

    vlayout5->addWidget(slider_1k);
    vlayout5->addWidget(lb5);

    vlayout6->addWidget(slider_3k);
    vlayout6->addWidget(lb6);

    vlayout7->addWidget(slider_6k);
    vlayout7->addWidget(lb7);

    vlayout8->addWidget(slider_12k);
    vlayout8->addWidget(lb8);

    vlayout9->addWidget(slider_14k);
    vlayout9->addWidget(lb9);

    vlayout10->addWidget(slider_16k);
    vlayout10->addWidget(lb10);

    auto hlayout  = new QHBoxLayout;
    hlayout->addLayout(vlayout0);
    hlayout->addLayout(vlayout1);
    hlayout->addLayout(vlayout2);
    hlayout->addLayout(vlayout3);
    hlayout->addLayout(vlayout4);
    hlayout->addLayout(vlayout5);
    hlayout->addLayout(vlayout6);
    hlayout->addLayout(vlayout7);
    hlayout->addLayout(vlayout8);
    hlayout->addLayout(vlayout9);
    hlayout->addLayout(vlayout10);
    hlayout->setSpacing(5);

    slWidget = new DBlurEffectWidget;
    slWidget->setBlurRectXRadius(8);
    slWidget->setBlurRectYRadius(8);
    slWidget->setRadius(8);
    slWidget->setBlurEnabled(true);
    slWidget->setMode(DBlurEffectWidget::GaussianBlur);
    slWidget->setMinimumWidth(517);
    slWidget->setMinimumHeight(264);
    slWidget->setLayout(hlayout);
    slWidget->setContentsMargins(10, 10, 9, 20);

    auto vlay  = new QVBoxLayout;
    //第一行
    vlay->addWidget(mtitleLabel);
    //第二行
    auto hlay  = new QHBoxLayout;
    hlay->addWidget(mswitchLabel, Qt::AlignLeft);
    hlay->addWidget(mswitchBtn, Qt::AlignLeft);
    hlay->addWidget(mcombox, 20, Qt::AlignCenter);
    hlay->addWidget(saveBtn, Qt::AlignRight);

    //第三行
    vlay->addLayout(hlay);
    vlay->addStretch(2);
    vlay->addWidget(slWidget);
    vlay->setContentsMargins(20, 10, 20, 20);

    //均衡器页面
    auto mequalizer = new DWidget(q);
    mequalizer->resize(537, 393);
    mequalizer->setLayout(vlay);

    auto mtabwidget = new DTabWidget;
    mtabwidget->setTabPosition(QTabWidget::West);
    mtabwidget->setAutoFillBackground(true);
    mtabwidget->setDocumentMode(true);
    mtabwidget->resize(720, 463);
    mtabwidget->addTab(mequalizer, "均衡器");
    mtabwidget->tabBar()->setStyle(new CustomTabStyle);
    mtabwidget->tabBar()->setContentsMargins(10, 0, 10, 0);

    auto mTitlebar = new DTitlebar(q);
    mTitlebar->setTitle("");
    mTitlebar->resize(720, 50);

    //垂直布局TabWidget和TitleBar
    auto  mlayout = new QVBoxLayout;
    mlayout->addWidget(mTitlebar);
    mlayout->addWidget(mtabwidget);
    mlayout->setSpacing(0);
    mlayout->setMargin(0);
    q->setLayout(mlayout);

    mcombox->setCurrentIndex(curEffect);
    q->enabledUI(flag);
    if (curEffect == 0) {
        q->customMode();
    }

}

//开机后默认参数设置
void DequalizerDialog::readConfig(MusicSettings *settings)
{
    Q_D(DequalizerDialog);
    d->switch_flag = settings->value("equalizer.all.switch").toBool();
    d->curEffect   = settings->value("equalizer.all.curEffect").toInt();

}

void DequalizerDialog::writeConfig(MusicSettings *settings)
{
    Q_D(DequalizerDialog);
    settings->setOption("equalizer.all.frequency_pre", d->slider_pre->value());
    settings->setOption("equalizer.all.frequency_60", d->slider_60->value());
    settings->setOption("equalizer.all.frequency_170", d->slider_170->value());
    settings->setOption("equalizer.all.frequency_310", d->slider_310->value());
    settings->setOption("equalizer.all.frequency_600", d->slider_600->value());
    settings->setOption("equalizer.all.frequency_1K", d->slider_1k->value());
    settings->setOption("equalizer.all.frequency_3K", d->slider_3k->value());
    settings->setOption("equalizer.all.frequency_6K", d->slider_6k->value());
    settings->setOption("equalizer.all.frequency_12K", d->slider_12k->value());
    settings->setOption("equalizer.all.frequency_14K", d->slider_14k->value());
    settings->setOption("equalizer.all.frequency_16K", d->slider_16k->value());
}

//UI使能
void DequalizerDialog::enabledUI(bool flag)
{
    Q_D(DequalizerDialog);
    d->mswitchBtn->setChecked(flag);
    d->mcombox->setEnabled(flag);
    for (DSlider *slider : findChildren<DSlider *>()) {
        slider->setEnabled(flag);
    }

    if (flag && d->curEffect == 0) {
        d->saveBtn->setEnabled(true);
    } else {
        d->saveBtn->setEnabled(false);
    }
}

void DequalizerDialog::initConnection()
{
    Q_D(DequalizerDialog);
    //均衡器使能
    connect(d->mswitchBtn, &DSwitchButton::checkedChanged, this, &DequalizerDialog::checkedChanged);
    // Create local connections
    connect(d->mcombox, QOverload<int>::of(&DComboBox::currentIndexChanged),
    this, [ = ](int index) {

        if (index == 0) {
            //“自定义”
            d->saveBtn->setEnabled(true);
            Q_EMIT getCurIndex(18);
        } else {
            d->saveBtn->setEnabled(false);
            Q_EMIT getCurIndex(index - 1);
        }
        d->curEffect = index;
    });
    //Switch mode
    connect(this, &DequalizerDialog::getCurIndex, d->_mediaPlayer->equalizer(), &VlcEqualizer::loadFromPreset);
    //display after switch mode
    connect(d->_mediaPlayer->equalizer(), &VlcEqualizer::presetLoaded, this, &DequalizerDialog::applySelectedPreset);

    applySelectedPreset();

    //set pream
    connect(d->slider_pre, &DSlider::valueChanged, d->_vlcEqualizer, &VlcEqualizer::setPreamplification);
    //set other slider
    for (DSlider *slider : findChildren<DSlider *>()) {
        if (slider != d->slider_pre) {
            connect(slider, &DSlider::valueChanged, this, &DequalizerDialog::applyChangesForBand);
        }
    }
    connect(d->saveBtn, &DPushButton::clicked,
    this, [ = ]() {
        writeConfig(d->settings);
        qDebug() << "保存";
    });
}

DequalizerDialog::DequalizerDialog(QWidget *parent):
    DAbstractDialog(parent), d_ptr(new DequalizerDialogPrivate(this))
{
    Q_D(DequalizerDialog);
    readConfig(d->settings);
    d->initUI(d->switch_flag);
}

DequalizerDialog::~DequalizerDialog()
{
//    Q_D(DequalizerDialog);
}

void DequalizerDialog::updateSettings()
{
    Q_D(DequalizerDialog);
    d->settings->setOption("equalizer.all.switch", d->switch_flag);
    d->settings->setOption("equalizer.all.curEffect", d->curEffect);
}

void DequalizerDialog::setMediaPlayer(VlcMediaPlayer *mediaPlayer)
{
    Q_D(DequalizerDialog);

    d->_mediaPlayer = mediaPlayer;
    if (!d->_mediaPlayer)
        return;
    d->_vlcEqualizer = d->_mediaPlayer->equalizer();
    initConnection();
}

void DequalizerDialog::applyChangesForBand(int value)
{
    Q_D(DequalizerDialog);
    int bandIndex = d->mapSliders.value(static_cast<DSlider *>(sender()));
    d->_mediaPlayer->equalizer()->setAmplificationForBandAt((float)value, bandIndex);
    d->mcombox->setCurrentIndex(0);
}

void DequalizerDialog::applySelectedPreset()
{
    Q_D(DequalizerDialog);
    auto equalizer = d->_mediaPlayer->equalizer();

//    disconnect(d->slider_pre, 0, equalizer, 0);
    for (DSlider *slider : findChildren<DSlider *>()) {
        if (slider == d->slider_pre) {
            slider->setValue(equalizer->preamplification());
        } else {
            disconnect(slider, &DSlider::valueChanged, this, &DequalizerDialog::applyChangesForBand);
            slider->setValue(equalizer->amplificationForBandAt(d->mapSliders.value(slider)));
            connect(slider, &DSlider::valueChanged, this, &DequalizerDialog::applyChangesForBand);
        }
    }
//    connect(d->slider_pre, &DSlider::valueChanged, equalizer, &VlcEqualizer::setPreamplification);
}

void DequalizerDialog::checkedChanged(bool checked)
{
    Q_D(DequalizerDialog);
    for (DSlider *slider : findChildren<DSlider *>()) {
        slider->setEnabled(checked);
    }
    d->switch_flag = checked;
    d->mcombox->setEnabled(checked);
    if (checked) {
        d->mcombox->setCurrentIndex(1);
    }
    d->_mediaPlayer->equalizer()->setEnabled(checked);
}
void DequalizerDialog::customMode()
{
    Q_D(DequalizerDialog);
    d->slider_pre->setValue(d->settings->value("equalizer.all.frequency_pre").toInt());
    d->slider_60->setValue(d->settings->value("equalizer.all.frequency_60").toInt());
    d->slider_170->setValue(d->settings->value("equalizer.all.frequency_170").toInt());
    d->slider_310->setValue(d->settings->value("equalizer.all.frequency_310").toInt());
    d->slider_600->setValue(d->settings->value("equalizer.all.frequency_600").toInt());
    d->slider_1k->setValue(d->settings->value("equalizer.all.frequency_1K").toInt());
    d->slider_3k->setValue(d->settings->value("equalizer.all.frequency_3K").toInt());
    d->slider_6k->setValue(d->settings->value("equalizer.all.frequency_6K").toInt());
    d->slider_12k->setValue(d->settings->value("equalizer.all.frequency_12K").toInt());
    d->slider_14k->setValue(d->settings->value("equalizer.all.frequency_14K").toInt());
    d->slider_16k->setValue(d->settings->value("equalizer.all.frequency_16K").toInt());
}
