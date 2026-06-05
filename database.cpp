#include "MainWindow.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QDialog>
#include <QMessageBox>
#include <QGraphicsBlurEffect>
#include <QIntValidator>
#include <QTimer>
#include <QMap>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QStandardPaths>
#include <QDir>

static const QMap<QString, QString> STUDENT_MAP = {
    {"251401001",  "Restu Syafiq"},
    {"251401002",  "Naufal Rasyiq Dziqri Simanjuntak"},
    {"251401003",  "Muhammad Alif Rafi"},
    {"251401004",  "Muhammad Fadhlan Siregar"},
    {"251401005",  "Mella Vonia"},
    {"251401006",  "Jibran Diaz"},
    {"251401007",  "Jose Revalino Sihite"},
    {"251401008",  "Kurnia Hafiz Arrasyid"},
    {"251401009",  "Keke Gemintang Padang"},
    {"251401010",  "Debora Graselia Pasaribu"},
    {"251401011",  "Friska Fadilah Simatupang"},
    {"251401012",  "Raudatul Janna"},
    {"251401013",  "Said Zaky Nasrullah"},
    {"251401014",  "Meisrik Wahyuni"},
    {"251401015",  "Salsabila Adriani Br Sembiring"},
    {"251401016",  "Mutiara Yuni"},
    {"251401017",  "Muhammad Fadlan Nasution"},
    {"251401018",  "Rafly Anthony Chaniago"},
    {"251401019",  "Hamdiyah Alifia Marsya Harahap"},
    {"251401020",  "Moza Apriza Parinduri"},
    {"251401021",  "Ika Safira Nst"},
    {"251401022",  "Natanael Marbun Lumbangaol"},
    {"251401023",  "Bunga Lestari"},
    {"251401024",  "Alawy Ali Zumaini"},
    {"251401025",  "Yuvensius Motifasi Daeli"},
    {"251401026",  "Naila Azzahra"},
    {"251401027",  "Alia Fitri Azkia"},
    {"251401028",  "Shofy Alika Nataya"},
    {"251401029",  "Jesslyn Inggrid"},
    {"251401030",  "Bellanice Bonifacia Siahaan"},
    {"251401031",  "Fahroel Munawar"},
    {"251401032",  "Melati Sekar Harum"},
    {"25140133",   "Rio Ganda Pahala Sianipar"},
    {"2514101034", "Yafi Khairohid"},
    {"251401035",  "Raziqul Athar"},
    {"251401036",  "Khalis Tofari"},
    {"251401037",  "Tora Eleazar Tarigan"},
    {"251401038",  "Jay Predyta Meliala"},
    {"251401039",  "Tengku Hafiz Novandi"},
    {"251401040",  "Athallah Muhammad Maajid"},
    {"251401041",  "Baktiar Lammartua Purba"},
    {"251401042",  "Muhammad Raffi Ghifari Razly"},
    {"251401043",  "M. Daffa Naufaldi Sitanggang"},
    {"251401044",  "Rizqi Sulthan Zaki"},
    {"251401045",  "Al Bima Rasya"},
    {"251401046",  "Arie Ardinata Kusuma"},
    {"251401047",  "Gabrielle Christin Gilberyn Ginting"},
    {"251401048",  "Maria Mekaliani Br Sinulingga"},
    {"251401049",  "Ahmad Royhan Harahap"},
    {"251401050",  "Dzaki Haikal Buchari"},
    {"251401051",  "Syahsabili Fithra Nasution"},
    {"251401052",  "Jhon Piter Situmorang"},
    {"251401053",  "Arfah Madany"},
    {"251401054",  "D. R. Monika Vita Gratia Damanik"},
    {"251401055",  "Muhammad Alfathi"},
    {"251401056",  "Amirah Larasati Sitorus"},
    {"251401057",  "Willyam Nicholas Siahaan"},
    {"251401058",  "Joshua Doharman Sinaga"},
    {"251401059",  "Geofrey Melvern"},
    {"251401060",  "Yehezkiel Saragih"},
    {"251401061",  "M. Axel Daffa Ramadhan"},
    {"251401062",  "Ezer Nicholas Purba"},
    {"251401063",  "Aldyra Siti Rachmah"},
    {"251401064",  "Anasti Fitri Ramadhani"},
    {"251401065",  "Aditya Nugraha"},
    {"251401066",  "Arcellya Abwannur"},
    {"251401067",  "Aini Raisyah Ridho Harahap"},
    {"251401068",  "Addin Syahid Nugraha"},
    {"251401069",  "Adrian Ricardo Sipahutar"},
    {"251401070",  "Aiyla Nazneen Ramadhan Hutapea"},
    {"251401071",  "Febrian Al-Katammi"},
    {"251401072",  "Hesekiel Silitonga"},
    {"251401073",  "Humika Richardo Immanuel"},
    {"251401074",  "Rizqa Faiza Raihanah"},
    {"251401075",  "Christian Isaac Wong"},
    {"251401076",  "Fernandez"},
    {"251401077",  "Ferdinand Macalister"},
    {"251401078",  "Oswinov Hutabarat"},
    {"251401079",  "Haura Fahimah Az Zahra"},
    {"251401080",  "Aksa Prawira Sukma"},
    {"251401081",  "Putri Fadilah Sari Pohan"},
    {"251401082",  "Zwinly Panjaitan"},
    {"251401083",  "Sri Rezeki Marito Manalu"},
    {"251401084",  "Naomi Virginia Lubis"},
    {"251401085",  "Alfandy Foncesa Simanjorang"},
    {"251401086",  "Salsabila Sujarwo"},
    {"251401087",  "Fazaallu Thaariq Amri"},
    {"251401088",  "Humairo Hendrifa"},
    {"251401089",  "Daniel Audrey Juanito Hutagalung"},
    {"251401090",  "Admaja Janias Nadhif Lahagu"},
    {"251401092",  "Wan Azzahra Putri Zalfi"},
    {"251401093",  "Daffa Fadhillah Reika"},
    {"251401094",  "Nadia Aulia Putri Nasution"},
    {"251401095",  "Owen Liandra"},
    {"251401096",  "Muzayyan Hafizh Nasution"},
    {"251401097",  "Dirza Zulhifzi"},
    {"251401098",  "Zu Alfarizi Lubis"},
    {"251401099",  "Ahmad Zaqi Alexandre Arrauf"},
    {"251401100",  "Surajan Sahadutha Sembiring"},
    {"251401102",  "Hasby Zaldy"},
    {"251401103",  "Raina Zhafira"},
    {"251401104",  "Alpin Maica Pratama"},
    {"251401105",  "Raduola Ginting"},
    {"251401106",  "Echa Yunda Rahayu"},
    {"251401107",  "Oryza Aulia Ramadhan Anafin"},
    {"251401108",  "Muhammad Naufal Fairuz"},
    {"251401109",  "Ilham Kurniansyah"},
    {"251401110",  "Billiam Agsa Tatema Hia"},
    {"251401111",  "Teguh Liandy"},
    {"251401112",  "Carlos Annacondia Meliala"},
    {"251401113",  "Immanuel M. Siringoringo"},
    {"251401114",  "Randa Agung Sembiring"},
    {"251401115",  "M. Fakhri Hamizan"},
    {"251401116",  "Ibnu Ad'ha Hasibuan"},
    {"251401117",  "Salim Ar-Rayyan Thirza"},
    {"251401118",  "Dustin Marvin John Lumbantobing"},
    {"251401120",  "Auzi Maalik Sihombing"},
    {"251401121",  "Daniel Sinurat"},
    {"251401122",  "Rafly Syahputra"},
    {"251401123",  "Frederico Manuel Sembiring"},
    {"251401124",  "Houra Farrin Pulungan"},
    {"251401125",  "Mayong Aditya Pradana"},
    {"251401126",  "Risky Rivaldzi Lubis"},
    {"251401127",  "Muhammad Alan Fatagifari"},
    {"251401128",  "Dwaia Prananta Ginting"},
    {"251401129",  "Dimas Dafa Fahreza"},
    {"251401131",  "O.K. Muhammad Hafiz"},
    {"251401132",  "Mario O.M. Sinaga"},
    {"251401133",  "Lionel Hiskia Hutabarat"},
    {"251401134",  "Frans Eikel Martogi Simanullang"},
    {"251401135",  "Irayani Kalilah Harahap"},
    {"251401136",  "Hans Kevin Austin Habeahan"},
    {"251401137",  "Azriel Zaidan Arva"},
    {"251401138",  "Jihan Firyal Husn Siregar"},
    {"251401139",  "Azka Reifan Yusyfa Nasution"},
    {"251401140",  "Hafifuddin"},
    {"251401141",  "Rd. M. Gharin Shauqi"},
    {"251401142",  "Ridho Pandapotan Pulungan"},
    {"251401143",  "Adinda Egia Rehulina Pelawi"},
    {"251401144",  "Muhammad Sandi"},
    {"251401145",  "Bimo Arya Prayugo"},
    {"251401147",  "Anggun Lizza Anggraini"},
    {"251401148",  "Raffa Fadhlan Tsalasa Lubis"},
    {"251401149",  "Andhika Rianda Harahap"},
    {"251401150",  "Indah Khairani Pasaribu"},
};

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    resize(1400, 800);
    initDatabase();

    stackedWidget = new QStackedWidget(this);
    homePage      = createHomePage();
    durationPage  = createDurationPage();

    stackedWidget->addWidget(homePage);
    stackedWidget->addWidget(durationPage);

    setCentralWidget(stackedWidget);
    setWindowTitle("Eceknya Type");
}

MainWindow::~MainWindow()

{
    if (isLoggedIn && !currentNIM.isEmpty())
        saveUserStats(currentNIM);

    if (db.isOpen()) db.close();
}

void MainWindow::initDatabase()
{
    QString dataPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir().mkpath(dataPath);

    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(dataPath + "/eceknya.db");

    if (!db.open()) {
        QMessageBox::critical(nullptr, "Database Error",
                            "Tidak bisa membuka database:\n" + db.lastError().text());
        return;
    }

    QSqlQuery q(db);

    q.exec(R"(
        CREATE TABLE IF NOT EXISTS users (
            nim              TEXT    PRIMARY KEY,
            highest_wpm      INTEGER DEFAULT 0,
            highest_accuracy REAL    DEFAULT 0.0,
            total_tests      INTEGER DEFAULT 0
        )
    )");
}

bool MainWindow::nimExists(const QString &nim)
{
    QSqlQuery q(db);
    q.prepare("SELECT COUNT(*) FROM users WHERE nim = :nim");
    q.bindValue(":nim", nim);
    if (q.exec() && q.next()) return q.value(0).toInt() > 0;
    return false;
}

void MainWindow::registerNIM(const QString &nim)
{
    QSqlQuery q(db);
    q.prepare("INSERT INTO users (nim) VALUES (:nim)");
    q.bindValue(":nim", nim);
    q.exec();
}

void MainWindow::loadUserStats(const QString &nim)
{
    QSqlQuery q(db);
    q.prepare("SELECT highest_wpm, highest_accuracy, total_tests FROM users WHERE nim = :nim");
    q.bindValue(":nim", nim);
    if (q.exec() && q.next()) {
        highestWPM      = q.value(0).toInt();
        highestAccuracy = q.value(1).toDouble();
        totalTests      = q.value(2).toInt();
    }
}

void MainWindow::saveUserStats(const QString &nim)
{
    QSqlQuery q(db);
    q.prepare(R"(
        UPDATE users
        SET highest_wpm      = :wpm,
            highest_accuracy = :acc,
            total_tests      = :tests
        WHERE nim = :nim
    )");
    q.bindValue(":wpm",   highestWPM);
    q.bindValue(":acc",   highestAccuracy);
    q.bindValue(":tests", totalTests);
    q.bindValue(":nim",   nim);
    q.exec();
}

QString MainWindow::nameFromNIM(const QString &nim)
{
    return STUDENT_MAP.value(nim, "");
}

void MainWindow::showLoginPopup()
{
    QString cardBg   = isDarkMode ? "#1E2130" : "white";
    QString border   = isDarkMode ? "#2E3350" : "#E5E7EB";
    QString textMain = isDarkMode ? "#E2E8F0" : "#111827";
    QString textSub  = isDarkMode ? "#94A3B8" : "#64748B";
    QString inputBg  = isDarkMode ? "#252840" : "#F9FAFB";


    QGraphicsBlurEffect *blur = new QGraphicsBlurEffect;
    blur->setBlurRadius(10);
    stackedWidget->setGraphicsEffect(blur);

    QDialog dlg(this);
    dlg.setModal(true);
    dlg.setFixedSize(420, 295);
    dlg.setWindowFlags(Qt::FramelessWindowHint);
    dlg.setStyleSheet(QString(
        "QDialog { background:%1; border-radius:24px; }"
        "QLabel  { color:%2; background:transparent; border:none; }"
    ).arg(cardBg, textMain));

    auto *vl = new QVBoxLayout(&dlg);
    vl->setContentsMargins(32, 14, 32, 28);

    auto *topRow  = new QHBoxLayout;
    auto *closeBtn = new QPushButton("✕");
    closeBtn->setFixedSize(34, 34);
    closeBtn->setStyleSheet(QString(
        "QPushButton { background:transparent; border:none; font-size:18px;"
        "  font-weight:bold; color:%1; }"
        "QPushButton:hover { color:%2; background:%3; border-radius:9px; }"
    ).arg(textSub, textMain, isDarkMode ? "#2E3350" : "#F3F4F6"));
    connect(closeBtn, &QPushButton::clicked, &dlg, &QDialog::reject);
    topRow->addStretch();
    topRow->addWidget(closeBtn);
    vl->addLayout(topRow);

    auto *ttl = new QLabel("Login");
    ttl->setAlignment(Qt::AlignCenter);
    ttl->setStyleSheet("font-size:30px; font-weight:800;");

    auto *sub = new QLabel(isEnglish ? "Please enter your Student ID" : "Silahkan masukkan NIM anda");
    sub->setAlignment(Qt::AlignCenter);
    sub->setStyleSheet(QString("font-size:14px; color:%1;").arg(textSub));

    auto *nimInput = new QLineEdit;
    nimInput->setValidator(new QIntValidator(0, 999999999, &dlg));
    nimInput->setPlaceholderText(isEnglish ? "Enter Student ID" : "Masukkan NIM");
    nimInput->setFixedHeight(48);
    nimInput->setStyleSheet(QString(
        "QLineEdit { border:2px solid %1; border-radius:12px;"
        "  padding-left:14px; font-size:15px;"
        "  background:%2; color:%3; font-weight:600; }"
        "QLineEdit:focus { border-color:%4; background:%5; }"
    ).arg(border, inputBg, textMain,
        isDarkMode ? "#6366F1" : "#60A5FA",
        isDarkMode ? "#252840" : "white"));
    QTimer::singleShot(0, nimInput, [nimInput]() { nimInput->setFocus(); });

    auto *statusLbl = new QLabel;
    statusLbl->setAlignment(Qt::AlignCenter);
    statusLbl->setStyleSheet(QString("font-size:12px; color:%1;").arg(textSub));

    connect(nimInput, &QLineEdit::textChanged, this, [this, statusLbl](const QString &t) {
        if (t.isEmpty()) { statusLbl->clear(); return; }
        if (nameFromNIM(t).isEmpty()) {

            statusLbl->setStyleSheet("font-size:12px; color:#EF4444;");
            statusLbl->setText(isEnglish ? "✘  NIM not found" : "✘  NIM tidak ditemukan");
        } else if (nimExists(t)) {

            statusLbl->setStyleSheet("font-size:12px; color:#4ADE80;");
            statusLbl->setText(isEnglish ? "✔️  Welcome back!" : "✔️  Selamat datang kembali!");
        } else {
    
            statusLbl->setStyleSheet("font-size:12px; color:#60A5FA;");
            statusLbl->setText(isEnglish ? "✦  New account will be created"
                                        : "✦  Akun baru akan dibuat");
        }
    });

    auto *loginBtn = new QPushButton("Login");
    loginBtn->setFixedHeight(48);
    QString btnBg  = isDarkMode ? "#6366F1" : "#05052E";
    QString btnHov = isDarkMode ? "#4F46E5" : "#11114A";
    loginBtn->setStyleSheet(QString(
        "QPushButton { background:%1; color:white; font-size:16px;"
        "  font-weight:700; border-radius:12px; border:none; }"
        "QPushButton:hover { background:%2; }"
    ).arg(btnBg, btnHov));

    connect(nimInput, &QLineEdit::returnPressed, loginBtn, &QPushButton::click);

    connect(loginBtn, &QPushButton::clicked, this, [&]() {
        QString nim = nimInput->text().trimmed();

        if (nim.isEmpty()) {
            QMessageBox::warning(&dlg, "Error",
                                isEnglish ? "Student ID cannot be empty!"
                                        : "NIM tidak boleh kosong!");
            return;
        }

        QString name = nameFromNIM(nim);
        if (name.isEmpty()) {
            QMessageBox::warning(&dlg, "Error",
                                isEnglish ? "NIM not found, please try again."
                                        : "NIM tidak ditemukan, silahkan masukkan lagi.");
            nimInput->clear();
            nimInput->setFocus();
            return;
        }

        if (!nimExists(nim)) registerNIM(nim);
        loadUserStats(nim);

        isLoggedIn  = true;
        currentNIM  = nim;
        currentName = name;
        if (loginProfileBtn) loginProfileBtn->setText("👤  Profile");
        dlg.accept();
    });

    vl->addStretch();
    vl->addWidget(ttl);
    vl->addSpacing(4);
    vl->addWidget(sub);
    vl->addSpacing(14);
    vl->addWidget(nimInput);
    vl->addSpacing(4);
    vl->addWidget(statusLbl);
    vl->addSpacing(12);
    vl->addWidget(loginBtn);
    vl->addStretch();

    dlg.exec();
    stackedWidget->setGraphicsEffect(nullptr);
}
