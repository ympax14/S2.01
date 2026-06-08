#include "DetailsWidget.hpp"
#include "./utils/Network.hpp"

#include <QLabel>
#include <QLineEdit>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QComboBox>
#include <QMetaEnum>
#include <QPushButton>
#include <QFileDialog>
#include <QHBoxLayout>
#include <QScrollArea>

#include "NowPlayingWindow.hpp"

DetailsWidget::DetailsWidget(QWidget *parent) :
    QWidget{parent},
    mainLayout(new QVBoxLayout(this)),
    informationsLayout(new QFormLayout),
    notesEdit(new QTextEdit(this)),
    imageScene(new QGraphicsScene(this)),
    imageView(new QGraphicsView(this))
{
    QScrollArea *scrollArea = new QScrollArea(this);
    QWidget *scrollContent = new QWidget;
    QVBoxLayout *scrollLayout = new QVBoxLayout(scrollContent);

    imageView->setScene(imageScene);
    imageView->setFixedHeight(300);
    imageView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    imageView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    notesEdit->setPlaceholderText(tr("Personal notes..."));
    notesEdit->setFixedHeight(100);
    notesEdit->hide();

    scrollLayout->addWidget(imageView);
    scrollLayout->addLayout(informationsLayout);
    scrollLayout->addWidget(notesEdit);
    scrollLayout->addStretch();

    scrollContent->setLayout(scrollLayout);
    scrollArea->setWidget(scrollContent);
    scrollArea->setWidgetResizable(true);
    mainLayout->addWidget(scrollArea);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    setLayout(mainLayout);
}

void DetailsWidget::loadImage(const QString& path) {
    this->imageScene->clear();

    QPixmap pixmap;
    if (Network::isUrl(path))
        pixmap = Network::fetchImage(path);
    else pixmap.load(path);

    if (!pixmap.isNull()) {
        pixmap = pixmap.scaledToHeight(this->imageView->height(), Qt::SmoothTransformation);
    } else {
        NowPlayingWindow* window = dynamic_cast<NowPlayingWindow*>(this->parentWidget()->parentWidget()); // On remonte de deux niveaux car splitter -> window
        assert(window != nullptr); // Présent seulement avec les symboles de déboguages, check de "précaution"
        window->errorToast("Album Cover", "Invalid Image");
    }

    this->imageScene->addPixmap(pixmap);
    this->imageScene->setSceneRect(pixmap.rect());
}

void DetailsWidget::clearImage() {
    this->imageScene->clear();
}

static void clearFormLayout(QFormLayout *layout) {
    while (layout->rowCount() > 0)
        layout->removeRow(0);
}

void DetailsWidget::buildAlbumDetails(Album* album) {
    clearFormLayout(informationsLayout);

    if (!album->getImageCover().isEmpty())
        this->loadImage(album->getImageCover());
    else
        this->clearImage();

    QLineEdit* titleEdit = new QLineEdit(album->getTitle());
    QObject::connect(titleEdit, &QLineEdit::textChanged, [this, album](const QString& v) {
        album->setTitle(v);
        emit dataChanged();
    });

    this->informationsLayout->addRow(new QLabel(tr("Title:")), titleEdit);

    // field for image url
    QLineEdit* imageEdit = new QLineEdit(album->getImageCover());
    imageEdit->setPlaceholderText(tr("Image URL / file path..."));
    QObject::connect(imageEdit, &QLineEdit::textChanged, [this, album](const QString& v) {
        album->setImageCover(v);
        this->loadImage(v); // puts image up to date, synch
        emit dataChanged();
    });

    // browse button to pick a local file
    QPushButton* browseButton = new QPushButton(tr("Browse..."));
    QObject::connect(browseButton, &QPushButton::clicked, [this, album, imageEdit]() {
        QString filePath = QFileDialog::getOpenFileName(
            this, tr("Select image"),
            QDir::homePath(),tr("Images (*.png *.jpg *.jpeg *.bmp *.gif)")
            );
        if (!filePath.isEmpty()) { imageEdit->setText(filePath); }
    });

    QHBoxLayout* imageLayout = new QHBoxLayout;
    imageLayout->addWidget(imageEdit);
    imageLayout->addWidget(browseButton);

    this->informationsLayout->addRow(new QLabel(tr("Image:")), imageLayout);

    QLineEdit* artistEdit = new QLineEdit(album->getArtistName());

    QObject::connect(artistEdit, &QLineEdit::textChanged, [this, album](const QString& v) {
        album->setArtistName(v);
        emit dataChanged();
    });

    this->informationsLayout->addRow(new QLabel(tr("Artist:")), artistEdit);

    QLineEdit* compositorEdit = new QLineEdit(album->getCompositorName());

    QObject::connect(compositorEdit, &QLineEdit::textChanged, [this, album](const QString& v) {
        album->setCompositorName(v);
        emit dataChanged();
    });

    this->informationsLayout->addRow(new QLabel(tr("Compositor:")), compositorEdit);

    QComboBox* categoryCombo = new QComboBox;
    const QMetaEnum metaCategory = QMetaEnum::fromType<Album::Category>();

    for (int i = 0; i < metaCategory.keyCount(); i++)
        categoryCombo->addItem(metaCategory.key(i));

    categoryCombo->setCurrentIndex(static_cast<int>(album->getCategory()));
    QObject::connect(categoryCombo, &QComboBox::currentIndexChanged, [this, album](int i) {
        album->setCategory(static_cast<Album::Category>(i));
        emit dataChanged();
    });

    this->informationsLayout->addRow(new QLabel(tr("Category:")), categoryCombo);

    QSpinBox* yearSpin = new QSpinBox;
    yearSpin->setRange(0, 9999);
    yearSpin->setValue(album->getEditionYear());

    QObject::connect(yearSpin, &QSpinBox::valueChanged, [this, album](int v) {
        album->setEditionYear(static_cast<quint16>(v));
        emit dataChanged();
    });

    this->informationsLayout->addRow(new QLabel(tr("Year:")), yearSpin);

    QComboBox* supportCombo = new QComboBox;
    const QMetaEnum metaSupport = QMetaEnum::fromType<Album::SupportType>();
    for (int i = 0; i < metaSupport.keyCount(); i++)
        supportCombo->addItem(metaSupport.key(i));
    supportCombo->setCurrentIndex(static_cast<int>(album->getSupportType()));

    QObject::connect(supportCombo, &QComboBox::currentIndexChanged, [this, album](int i) {
        album->setSupportType(static_cast<Album::SupportType>(i));
        emit dataChanged();
    });

    this->informationsLayout->addRow(new QLabel(tr("Support:")), supportCombo);

    QDoubleSpinBox* priceSpin = new QDoubleSpinBox;
    priceSpin->setRange(0.0, 9999.99);
    priceSpin->setSuffix(tr(" €"));
    priceSpin->setDecimals(2);
    priceSpin->setValue(album->getEuroPrice());

    QObject::connect(priceSpin, &QDoubleSpinBox::valueChanged, [this, album](double v) {
        album->setEuroPrice(v);
        emit dataChanged();
    });

    this->informationsLayout->addRow(new QLabel(tr("Price:")), priceSpin);

    this->informationsLayout->addRow(new QLabel(tr("Songs:")), new QLabel(QString::number(album->getSongsCount())));

    // Prevents dataChanged to be emitted when setting the current notes value
    QObject::disconnect(this->notesEdit, nullptr, nullptr, nullptr);
    this->notesEdit->blockSignals(true);
    this->notesEdit->setPlainText(album->getPersonalNotes());
    this->notesEdit->blockSignals(false);

    QObject::connect(this->notesEdit, &QTextEdit::textChanged, [this, album]() {
        album->setPersonalNotes(this->notesEdit->toPlainText());
        emit dataChanged();
    });

    this->notesEdit->show();
}

void DetailsWidget::buildSongDetails(Album* const album, size_t songIndex) {
    clearFormLayout(informationsLayout);
    this->notesEdit->hide();

    if (!album->getImageCover().isEmpty())
        this->loadImage(album->getImageCover());
    else
        this->clearImage();

    QSpinBox* indexSpin = new QSpinBox;
    indexSpin->setRange(0, 255);
    indexSpin->setValue(album->getSong(songIndex).getIndex());

    QObject::connect(indexSpin, &QSpinBox::valueChanged, [this, album, songIndex](int v) {
        album->getSong(songIndex).setIndex(static_cast<quint16>(v));
        emit dataChanged();
    });

    this->informationsLayout->addRow(new QLabel(tr("Index:")), indexSpin);

    QLineEdit* titleEdit = new QLineEdit(album->getSong(songIndex).getTitle());

    QObject::connect(titleEdit, &QLineEdit::textChanged, [this, album, songIndex](const QString& v) {
        album->getSong(songIndex).setTitle(v);
        emit dataChanged();
    });

    this->informationsLayout->addRow(new QLabel(tr("Title:")), titleEdit);

    QSpinBox* durationSpin = new QSpinBox;
    durationSpin->setRange(0, 65535);
    durationSpin->setSuffix(tr(" s"));
    durationSpin->setValue(album->getSong(songIndex).getDuration());

    QObject::connect(durationSpin, &QSpinBox::valueChanged, [this, album, songIndex](int v) {
        album->getSong(songIndex).setDuration(static_cast<quint16>(v));
        emit dataChanged();
    });

    this->informationsLayout->addRow(new QLabel(tr("Duration:")), durationSpin);

    QDoubleSpinBox* ratingSpin = new QDoubleSpinBox;
    ratingSpin->setRange(0.0, 5.0);
    ratingSpin->setSingleStep(0.5);
    ratingSpin->setDecimals(1);
    ratingSpin->setValue(album->getSong(songIndex).getRating());

    QObject::connect(ratingSpin, &QDoubleSpinBox::valueChanged, [this, album, songIndex](double v) {
        album->getSong(songIndex).setRating(v);
        emit dataChanged();
    });

    this->informationsLayout->addRow(new QLabel(tr("Rating:")), ratingSpin);
}
