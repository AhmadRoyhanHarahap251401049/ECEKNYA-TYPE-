// ========================== MainWindow.cpp ==========================

// ── Includes ──────────────────────────────────────────
#include "MainWindow.h"

#include <QApplication>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QFrame>
#include <QDialog>
#include <QLineEdit>
#include <QScrollArea>
#include <QMessageBox>
#include <QGraphicsBlurEffect>
#include <QIntValidator>
#include <QTimer>
#include <QRandomGenerator>
#include <QMap>

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QStandardPaths>
#include <QDir>

// ======================================================
// KAMUS KATA — Bahasa Indonesia
// ======================================================

static const QStringList KATA_ID = {
    // --- Kata Ganti & Kata Hubung ---
    "aku","kamu","dia","kita","mereka","dan","atau","tapi","jika","karena",
    // --- Kata Kerja Bantu ---
    "sudah","belum","akan","bisa","mau","perlu","harus","boleh","tidak","ya",
    // --- Tempat ---
    "rumah","sekolah","kantor","jalan","pasar","taman","kota","desa","laut","gunung",
    // --- Aktivitas ---
    "makan","minum","tidur","belajar","kerja","lari","bicara","dengar","lihat","masak",
    // --- Sifat ---
    "besar","kecil","tinggi","rendah","cepat","lambat","panas","dingin","baik","buruk",
    // --- Angka ---
    "satu","dua","tiga","empat","lima","enam","tujuh","delapan","sembilan","sepuluh",
    // --- Waktu ---
    "hari","bulan","tahun","waktu","pagi","siang","sore","malam","minggu","detik",
    // --- Alam ---
    "air","api","tanah","udara","cahaya","suara","warna","bentuk","ukuran","berat",
    // --- Orang ---
    "orang","anak","ibu","ayah","kakak","adik","teman","guru","dokter","polisi",
    // --- Benda ---
    "buku","pensil","kertas","meja","kursi","pintu","jendela","lantai","atap","dinding",
    // --- Hewan ---
    "kucing","anjing","burung","ikan","kuda","sapi","kambing","ayam","bebek","kelinci",
    // --- Makanan ---
    "buah","sayur","nasi","roti","mie","sup","kue","gula","garam","minyak",
    // --- Kendaraan ---
    "mobil","motor","sepeda","bus","kereta","kapal","pesawat","truk","becak","angkot",
    // --- Perasaan ---
    "senang","sedih","marah","takut","heran","malu","bangga","kecewa","lega","bosan",
    // --- Karakter ---
    "pintar","rajin","malas","jujur","sopan","kasar","sabar","hemat","berani","rendah",
    // --- Sifat Lainnya ---
    "indah","baru","lama","jauh","dekat","dalam","tebal","tipis","mudah","susah",
    // --- Kondisi ---
    "aman","bersih","kotor","ramai","sepi","terang","gelap","sehat","sakit","segar",
    // --- Gerakan ---
    "mulai","selesai","buka","tutup","masuk","keluar","naik","turun","datang","pergi",
    // --- Kata Kerja Kompleks ---
    "membaca","menulis","menyanyi","menari","memasak","membantu","menjaga","mencari",
    // --- Abstrak ---
    "harapan","impian","tujuan","rencana","usaha","hasil","manfaat","kepercayaan",
    // --- Sosial ---
    "persahabatan","keluarga","cinta","kasih","sayang","perhatian","hormat","kerja",
    // --- Teknologi ---
    "komputer","telepon","internet","aplikasi","program","data","informasi","teknologi",
    // --- Pendidikan ---
    "pelajaran","ujian","nilai","prestasi","juara","hadiah","penghargaan","sertifikat",
    // --- Negara & Budaya ---
    "negara","bangsa","budaya","bahasa","sejarah","tradisi","adat","norma","hukum",
    // --- Kesehatan ---
    "kesehatan","olahraga","istirahat","vitamin","gizi","energi","kekuatan","stamina",
    // --- Pemecahan Masalah ---
    "masalah","solusi","pertanyaan","jawaban","pendapat","diskusi","argumen","keputusan",
    // --- Inovasi ---
    "perubahan","kemajuan","inovasi","kreativitas","ide","gagasan","konsep","strategi"
};

// ======================================================
// KAMUS KATA — Bahasa Inggris
// ======================================================

static const QStringList KATA_EN = {
    // --- Pronouns & Conjunctions ---
    "the","a","an","and","or","but","if","because","when","while",
    // --- Verbs ---
    "run","walk","eat","sleep","write","read","talk","look","make","work",
    // --- Places ---
    "home","school","office","road","park","city","town","sea","mountain","forest",
    // --- Adjectives ---
    "big","small","tall","short","fast","slow","hot","cold","good","bad",
    // --- Numbers ---
    "one","two","three","four","five","six","seven","eight","nine","ten",
    // --- Time ---
    "day","month","year","time","morning","noon","evening","night","week","second",
    // --- Nature ---
    "water","fire","earth","air","light","sound","color","shape","size","weight",
    // --- People ---
    "person","child","mother","father","friend","teacher","doctor","student","brother","sister",
    // --- Objects ---
    "book","pen","paper","table","chair","door","window","floor","wall","roof",
    // --- Animals ---
    "cat","dog","bird","fish","horse","cow","goat","chicken","duck","rabbit",
    // --- Food ---
    "fruit","rice","bread","soup","cake","sugar","salt","oil","noodle","vegetable",
    // --- Vehicles ---
    "car","bike","bus","train","ship","plane","truck","boat","taxi","van",
    // --- Feelings ---
    "happy","sad","angry","scared","proud","bored","tired","excited","calm","worried",
    // --- Tech ---
    "code","data","app","web","system","server","network","program","software","device",
    // --- Actions ---
    "start","stop","open","close","enter","exit","push","pull","send","receive",
    // --- Descriptors ---
    "new","old","near","far","deep","thick","thin","easy","hard","clear",
    // --- Abstract ---
    "hope","dream","goal","plan","idea","effort","result","trust","change","power",
    // --- Education ---
    "lesson","test","score","study","learn","class","grade","exam","skill","knowledge",
    // --- Social ---
    "family","love","care","team","group","share","help","support","respect","connect",
    // --- Complex Verbs ---
    "running","writing","reading","singing","dancing","cooking","helping","watching","listening","thinking"
};

// ======================================================
// DAFTAR MAHASISWA — NIM → Nama
// ======================================================

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
    {"25140133",   "Rio Ganda Pahala Sianipar"},   // NIM sesuai daftar (kemungkinan typo)
    {"2514101034", "Yafi Khairohid"},              // NIM sesuai daftar (kemungkinan typo)
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

// ======================================================
// CONSTRUCTOR & DESTRUCTOR
// ======================================================

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
    // Simpan data user sebelum aplikasi ditutup
    if (isLoggedIn && !currentNIM.isEmpty())
        saveUserStats(currentNIM);

    if (db.isOpen()) db.close();
}

// ======================================================
// DATABASE — Inisialisasi & Query
// ======================================================

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

    // --- Tabel users ---
    q.exec(R"(
        CREATE TABLE IF NOT EXISTS users (
            nim              TEXT    PRIMARY KEY,
            highest_wpm      INTEGER DEFAULT 0,
            highest_accuracy REAL    DEFAULT 0.0,
            total_tests      INTEGER DEFAULT 0
        )
    )");
}

// --- Cek apakah NIM sudah terdaftar ---
bool MainWindow::nimExists(const QString &nim)
{
    QSqlQuery q(db);
    q.prepare("SELECT COUNT(*) FROM users WHERE nim = :nim");
    q.bindValue(":nim", nim);
    if (q.exec() && q.next()) return q.value(0).toInt() > 0;
    return false;
}

// --- Daftarkan NIM baru ---
void MainWindow::registerNIM(const QString &nim)
{
    QSqlQuery q(db);
    q.prepare("INSERT INTO users (nim) VALUES (:nim)");
    q.bindValue(":nim", nim);
    q.exec();
}

// --- Muat statistik user dari DB ---
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

// --- Simpan statistik ke DB ---
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

// --- Lookup nama dari daftar kelas ---
QString MainWindow::nameFromNIM(const QString &nim)
{
    return STUDENT_MAP.value(nim, "");   // "" jika tidak ditemukan
}

// ======================================================
// HELPERS — Rebuild home & duration page
// ======================================================

void MainWindow::rebuildMainPages()
{
    QWidget *prev       = stackedWidget->currentWidget();
    bool wasOnDuration  = (prev == durationPage);
    bool stayOnOther    = (prev != homePage && prev != durationPage);

    int homeIdx = stackedWidget->indexOf(homePage);
    stackedWidget->removeWidget(homePage);
    delete homePage;
    homePage = createHomePage();
    stackedWidget->insertWidget(homeIdx, homePage);

    int durIdx = stackedWidget->indexOf(durationPage);
    stackedWidget->removeWidget(durationPage);
    delete durationPage;
    durationPage = createDurationPage();
    stackedWidget->insertWidget(durIdx, durationPage);

    if      (stayOnOther)   stackedWidget->setCurrentWidget(prev);
    else if (wasOnDuration) stackedWidget->setCurrentWidget(durationPage);
    else                    stackedWidget->setCurrentWidget(homePage);
}

// ======================================================
// THEME — Toggle gelap / terang
// ======================================================

void MainWindow::toggleTheme()
{
    isDarkMode = !isDarkMode;
    rebuildMainPages();
    refreshWordDisplay();
}

// ======================================================
// LANGUAGE — Toggle ID / EN
// ======================================================

void MainWindow::toggleLanguage()
{
    isEnglish = !isEnglish;
    rebuildMainPages();
}

// ======================================================
// WORD LIST — Generator kata acak
// ======================================================

QStringList MainWindow::generateWordList(int seconds)
{
    const QStringList &pool = isEnglish ? KATA_EN : KATA_ID;
    int count = qMax(60, seconds * 3);
    int total = pool.size();
    QStringList list;
    for (int i = 0; i < count; ++i)
        list.append(pool[QRandomGenerator::global()->bounded(total)]);
    return list;
}

// ======================================================
// HOME PAGE
// ======================================================

QWidget *MainWindow::createHomePage()
{
    QWidget *page = new QWidget;
    page->setStyleSheet(
        isDarkMode ? "background-color:#0F1117;" : "background-color:#F5F7FB;"
        );

    QVBoxLayout *root = new QVBoxLayout(page);
    root->setContentsMargins(40, 20, 40, 20);

    // ── Navbar ───────────────────────────────────────────
    QHBoxLayout *navbar = new QHBoxLayout;

    QLabel *logo = new QLabel("Eceknya Type");
    logo->setStyleSheet(QString("font-size:26px; font-weight:800; color:%1;")
                            .arg(isDarkMode ? "#E2E8F0" : "#111827"));

    auto makeNavBtn = [this](const QString &text, bool dark = false) -> QPushButton* {
        auto *btn = new QPushButton(text);
        btn->setFixedHeight(40);
        if (dark) {
            QString bg  = isDarkMode ? "#6366F1" : "#05052E";
            QString hov = isDarkMode ? "#4F46E5" : "#11114A";
            btn->setStyleSheet(QString(
                                   "QPushButton { background:%1; color:white; border-radius:10px;"
                                   "  padding:6px 18px; font-size:14px; font-weight:700; border:none; }"
                                   "QPushButton:hover { background:%2; }"
                                   ).arg(bg, hov));
        } else {
            QString bg     = isDarkMode ? "#1E2130" : "white";
            QString border = isDarkMode ? "#2E3350" : "#E2E8F0";
            QString text2  = isDarkMode ? "#E2E8F0" : "#111827";
            QString hov    = isDarkMode ? "#2E3350" : "#F3F4F6";
            btn->setStyleSheet(QString(
                                   "QPushButton { background:%1; border:1px solid %2; border-radius:10px;"
                                   "  padding:6px 16px; font-size:13px; font-weight:600; color:%3; }"
                                   "QPushButton:hover { background:%4; }"
                                   ).arg(bg, border, text2, hov));
        }
        return btn;
    };

    themeBtn = makeNavBtn(isDarkMode
                              ? (isEnglish ? "☀  Light" : "☀  Terang")
                              : (isEnglish ? "🌙  Dark"  : "🌙  Gelap"));
    connect(themeBtn, &QPushButton::clicked, this, &MainWindow::toggleTheme);

    langBtn = makeNavBtn(isEnglish ? "🌐  ID" : "🌐  EN");
    connect(langBtn, &QPushButton::clicked, this, &MainWindow::toggleLanguage);

    loginProfileBtn = makeNavBtn(isLoggedIn ? "👤  Profile" : "👤  Login", true);
    connect(loginProfileBtn, &QPushButton::clicked, this, [this]() {
        if (isLoggedIn) showProfilePopup();
        else            showLoginPopup();
    });

    navbar->addWidget(logo);
    navbar->addStretch();
    navbar->addWidget(langBtn);
    navbar->addSpacing(8);
    navbar->addWidget(themeBtn);
    navbar->addSpacing(8);
    navbar->addWidget(loginProfileBtn);

    // ── Hero Section ──────────────────────────────────────
    QVBoxLayout *hero = new QVBoxLayout;
    hero->setAlignment(Qt::AlignCenter);

    QString heroTitleTxt = isEnglish
                               ? "Improve Your Typing Speed"
                               : "Tingkatkan Kecepatan Mengetikmu";
    QLabel *heroTitle = new QLabel(heroTitleTxt);
    heroTitle->setAlignment(Qt::AlignCenter);
    heroTitle->setStyleSheet(QString("font-size:48px; font-weight:800; color:%1;")
                                 .arg(isDarkMode ? "#E2E8F0" : "#111827"));

    QString heroDescTxt = isEnglish
                              ? "Practice typing with real-time feedback,\ntrack your progress and beat your records."
                              : "Latihan mengetik dengan umpan balik real-time,\npantau progresmu, dan bersaing dengan orang lain.";
    QLabel *heroDesc = new QLabel(heroDescTxt);
    heroDesc->setAlignment(Qt::AlignCenter);
    heroDesc->setStyleSheet(QString("font-size:19px; color:%1; line-height:1.6;")
                                .arg(isDarkMode ? "#94A3B8" : "#64748B"));

    QString startTxt = isEnglish ? "▶  Start Typing Test" : "▶  Mulai Tes Mengetik";
    QPushButton *startBtn = new QPushButton(startTxt);
    startBtn->setFixedSize(320, 65);
    QString btnBg  = isDarkMode ? "#6366F1" : "#05052E";
    QString btnHov = isDarkMode ? "#4F46E5" : "#11114A";
    startBtn->setStyleSheet(QString(
                                "QPushButton { background:%1; color:white; font-size:20px;"
                                "  font-weight:700; border-radius:14px; border:none; }"
                                "QPushButton:hover  { background:%2; }"
                                "QPushButton:pressed{ background:%2; }"
                                ).arg(btnBg, btnHov));
    connect(startBtn, &QPushButton::clicked, this, &MainWindow::showDurationPage);

    hero->addSpacing(30);
    hero->addWidget(heroTitle);
    hero->addSpacing(14);
    hero->addWidget(heroDesc);
    hero->addSpacing(32);
    hero->addWidget(startBtn, 0, Qt::AlignCenter);
    hero->addSpacing(50);

    // ── Feature Cards ─────────────────────────────────────
    struct Feature { QString icon, title, desc; };
    QList<Feature> features = {
                               { "🌐",
                                isEnglish ? "Multi Language"  : "Multiple Bahasa",
                                isEnglish ? "Hundreds of words\nin English & Indonesian" : "Ratusan kata bahasa\nIndonesia & Inggris yang beragam" },
                               { "📊",
                                isEnglish ? "Full Statistics" : "Statistik Lengkap",
                                isEnglish ? "Track your WPM\nand accuracy progress" : "Pantau progres WPM\ndan akurasi kamu" },
                               };

    QString cardBg     = isDarkMode ? "#1E2130" : "white";
    QString cardBorder = isDarkMode ? "#2E3350" : "#E5E7EB";
    QString cardText   = isDarkMode ? "#E2E8F0" : "#111827";
    QString cardSub    = isDarkMode ? "#94A3B8" : "#64748B";

    QHBoxLayout *cards = new QHBoxLayout;
    cards->setSpacing(24);
    cards->setAlignment(Qt::AlignCenter);
    for (auto &f : features) {
        QFrame *card = new QFrame;
        card->setFixedSize(280, 160);
        card->setStyleSheet(QString(
                                "background:%1; border:1px solid %2; border-radius:18px;"
                                ).arg(cardBg, cardBorder));
        QVBoxLayout *cl = new QVBoxLayout(card);
        cl->setAlignment(Qt::AlignCenter);
        cl->setSpacing(6);

        auto *ico = new QLabel(f.icon);
        ico->setAlignment(Qt::AlignCenter);
        ico->setStyleSheet("font-size:30px; background:transparent; border:none;");

        auto *ttl = new QLabel(f.title);
        ttl->setAlignment(Qt::AlignCenter);
        ttl->setStyleSheet(QString(
                               "font-size:16px; font-weight:700; color:%1;"
                               " background:transparent; border:none;"
                               ).arg(cardText));

        auto *dsc = new QLabel(f.desc);
        dsc->setAlignment(Qt::AlignCenter);
        dsc->setStyleSheet(QString(
                               "font-size:12px; color:%1; background:transparent; border:none;"
                               ).arg(cardSub));

        cl->addWidget(ico);
        cl->addWidget(ttl);
        cl->addWidget(dsc);
        cards->addWidget(card);
    }

    root->addLayout(navbar);
    root->addStretch();
    root->addLayout(hero);
    root->addLayout(cards);
    root->addStretch();

    return page;
}

// ======================================================
// DURATION PAGE
// ======================================================

QWidget *MainWindow::createDurationPage()
{
    QWidget *page = new QWidget;
    page->setStyleSheet(
        isDarkMode ? "background-color:#0F1117;" : "background-color:#F5F7FB;"
        );

    QVBoxLayout *root = new QVBoxLayout(page);
    root->setContentsMargins(40, 20, 40, 20);

    // ── Back Button ───────────────────────────────────────
    QHBoxLayout *topRow = new QHBoxLayout;
    QString backTxt = isEnglish ? "← Back" : "← Kembali";
    QPushButton *backBtn = new QPushButton(backTxt);
    backBtn->setFixedSize(130, 42);
    QString btnBg  = isDarkMode ? "#6366F1" : "#05052E";
    QString btnHov = isDarkMode ? "#4F46E5" : "#11114A";
    backBtn->setStyleSheet(QString(
                               "QPushButton { background:%1; color:white; border-radius:12px;"
                               "  font-size:14px; font-weight:700; border:none; }"
                               "QPushButton:hover { background:%2; }"
                               ).arg(btnBg, btnHov));
    connect(backBtn, &QPushButton::clicked, this, [this]() {
        stackedWidget->setCurrentWidget(homePage);
    });
    topRow->addStretch();
    topRow->addWidget(backBtn);

    // ── Title & Desc ──────────────────────────────────────
    QString titleTxt = isEnglish ? "Select Duration" : "Pilih Durasi Tes";
    QLabel *title = new QLabel(titleTxt);
    title->setAlignment(Qt::AlignCenter);
    title->setStyleSheet(QString("font-size:46px; font-weight:800; color:%1;")
                             .arg(isDarkMode ? "#E2E8F0" : "#111827"));

    QString descTxt = isEnglish
                          ? "Choose how long you want to practice typing"
                          : "Pilih berapa lama kamu ingin berlatih mengetik";
    QLabel *desc = new QLabel(descTxt);
    desc->setAlignment(Qt::AlignCenter);
    desc->setStyleSheet(QString("font-size:17px; color:%1;")
                            .arg(isDarkMode ? "#94A3B8" : "#64748B"));

    // ── Duration Cards ────────────────────────────────────
    struct DurationCard {
        QString sec, icon;
        QString bgLight, borderLight, hoverLight;
        QString bgDark,  borderDark,  hoverDark;
    };
    QList<DurationCard> durations = {
                                     { "15", "⚡", "#FFFBEB", "#FACC15", "#FEF9C3",  "#1E2130", "#4B4010", "#2A2A10" },
                                     { "30", "🎯", "#EFF6FF", "#60A5FA", "#DBEAFE",  "#1E2130", "#1E3A5F", "#162540" },
                                     { "60", "🔥", "#FAF5FF", "#D8B4FE", "#F3E8FF",  "#1E2130", "#3B2060", "#2A1545" },
                                     };

    QString labelTxt = isEnglish ? "SEC" : "DETIK";

    QHBoxLayout *cardRow = new QHBoxLayout;
    cardRow->setSpacing(30);
    for (auto &d : durations) {
        QPushButton *card = new QPushButton(
            QString("%1\n%2\n%3").arg(d.icon, d.sec, labelTxt)
            );
        card->setFixedSize(260, 280);

        QString cardBg  = isDarkMode ? d.bgDark     : d.bgLight;
        QString cardBdr = isDarkMode ? d.borderDark : d.borderLight;
        QString cardHov = isDarkMode ? d.hoverDark  : d.hoverLight;
        QString txtCol  = isDarkMode ? "#E2E8F0"    : "#111827";

        card->setStyleSheet(QString(
                                "QPushButton { background:%1; border:3px solid %2; border-radius:24px;"
                                "  font-size:46px; font-weight:800; color:%3; }"
                                "QPushButton:hover { background:%4; }"
                                ).arg(cardBg, cardBdr, txtCol, cardHov));

        int secs = d.sec.toInt();
        connect(card, &QPushButton::clicked, this, [this, secs]() {
            runTypingTest(secs);
        });
        cardRow->addWidget(card);
    }

    root->addLayout(topRow);
    root->addStretch();
    root->addWidget(title);
    root->addSpacing(8);
    root->addWidget(desc);
    root->addSpacing(48);
    root->addLayout(cardRow);
    root->addStretch();

    return page;
}

// ======================================================
// TYPING PAGE — Word Display
// ======================================================

void MainWindow::refreshWordDisplay()
{
    if (!wordDisplay) return;

    QString colorDone   = isDarkMode ? "#475569" : "#94A3B8";
    QString colorActive = isDarkMode ? "#E2E8F0" : "#1E293B";
    QString colorActBg  = isDarkMode ? "#2D3561" : "#DBEAFE";
    QString colorNext   = isDarkMode ? "#334155" : "#CBD5E1";

    QString html;
    int start = qMax(0, currentWordIndex - 10);
    int end   = qMin(wordList.size() - 1, currentWordIndex + 50);

    for (int i = start; i <= end; ++i) {
        const QString &word = wordList[i];

        if (i < currentWordIndex) {
            html += QString("<span style='color:%1;'>%2</span> ").arg(colorDone, word);
        }
        else if (i == currentWordIndex) {
            html += QString(
                        "<span style='color:%1; font-weight:700;"
                        " background-color:%2; border-radius:4px; padding:2px 5px;'>%3</span> "
                        ).arg(colorActive, colorActBg, word);
        }
        else {
            html += QString("<span style='color:%1;'>%2</span> ").arg(colorNext, word);
        }
    }

    wordDisplay->setText(html);
}

// ======================================================
// TYPING PAGE — Builder
// ======================================================

QWidget *MainWindow::createTypingPage(int seconds)
{
    QString pageBg   = isDarkMode ? "#0F1117" : "#F5F7FB";
    QString cardBg   = isDarkMode ? "#1E2130" : "white";
    QString border   = isDarkMode ? "#2E3350" : "#E5E7EB";
    QString textMain = isDarkMode ? "#E2E8F0" : "#111827";
    QString textMut  = isDarkMode ? "#64748B" : "#94A3B8";

    QWidget *page = new QWidget;
    page->setStyleSheet(QString("background-color:%1;").arg(pageBg));

    QVBoxLayout *root = new QVBoxLayout(page);
    root->setContentsMargins(60, 28, 60, 28);
    root->setSpacing(0);

    // ── Navbar ───────────────────────────────────────────
    QHBoxLayout *navbar = new QHBoxLayout;

    auto *navLogo = new QLabel("⌨  Eceknya Type");
    navLogo->setStyleSheet(QString("font-size:20px; font-weight:800; color:%1;").arg(textMain));

    auto *exitBtn = new QPushButton(isEnglish ? "✕  Exit" : "✕  Keluar");
    exitBtn->setFixedHeight(36);
    exitBtn->setStyleSheet(QString(
                               "QPushButton { background:%1; border:1px solid %2; border-radius:10px;"
                               "  padding:5px 14px; font-size:13px; font-weight:600; color:%3; }"
                               "QPushButton:hover { background:#FEF2F2; color:#DC2626; border-color:#FECACA; }"
                               ).arg(cardBg, border, textMain));
    connect(exitBtn, &QPushButton::clicked, this, [this]() {
        if (countdownTimer) countdownTimer->stop();
        if (isLoggedIn && !currentNIM.isEmpty()) saveUserStats(currentNIM);
        stackedWidget->setCurrentWidget(durationPage);
    });

    navbar->addWidget(navLogo);
    navbar->addStretch();
    navbar->addWidget(exitBtn);

    // ── Duration Badge ────────────────────────────────────
    QString badgeTxt = isEnglish
                           ? QString("Test Duration: %1 sec").arg(seconds)
                           : QString("Durasi Tes: %1 detik").arg(seconds);
    auto *badge = new QLabel(badgeTxt);
    badge->setAlignment(Qt::AlignCenter);
    badge->setFixedHeight(36);
    badge->setStyleSheet(QString(
                             "background:%1; border:1px solid %2; border-radius:16px;"
                             "font-size:14px; font-weight:600; color:%3; padding:3px 20px;"
                             ).arg(cardBg, border, textMain));

    // ── Stat Cards ────────────────────────────────────────
    auto makeStatCard = [&](QLabel **out, const QString &header,
                            const QString &init, const QString &unit) -> QFrame*
    {
        auto *card = new QFrame;
        card->setFixedHeight(106);
        card->setStyleSheet(QString(
                                "background:%1; border:1px solid %2; border-radius:16px;"
                                ).arg(cardBg, border));

        auto *vl = new QVBoxLayout(card);
        vl->setContentsMargins(16, 10, 16, 10);

        auto *hdr = new QLabel(header);
        hdr->setStyleSheet(QString(
                               "font-size:12px; font-weight:600; color:%1;"
                               " background:transparent; border:none;"
                               ).arg(textMut));

        auto *row = new QHBoxLayout;
        auto *val = new QLabel(init);
        val->setStyleSheet(QString(
                               "font-size:40px; font-weight:800; color:%1;"
                               " background:transparent; border:none;"
                               ).arg(textMain));
        auto *unitLbl = new QLabel(unit);
        unitLbl->setStyleSheet(QString(
                                   "font-size:14px; color:%1; font-weight:600;"
                                   " margin-left:4px; margin-top:14px; background:transparent; border:none;"
                                   ).arg(textMut));
        row->addWidget(val);
        row->addWidget(unitLbl);
        row->addStretch();

        vl->addWidget(hdr);
        vl->addLayout(row);

        if (out) *out = val;
        return card;
    };

    QLabel *wpmLbl = nullptr, *accLbl = nullptr, *timLbl = nullptr;
    auto *statsRow = new QHBoxLayout;
    statsRow->setSpacing(16);
    statsRow->addWidget(makeStatCard(&wpmLbl, "Speed",    "0",                      "WPM"));
    statsRow->addWidget(makeStatCard(&accLbl, "Accuracy", "100",                    "%"  ));
    statsRow->addWidget(makeStatCard(&timLbl, "Time",     QString::number(seconds), "sec"));

    wpmLabel      = wpmLbl;
    accuracyLabel = accLbl;
    timerLabel    = timLbl;

    // ── Word Display ──────────────────────────────────────
    auto *wordFrame = new QFrame;
    wordFrame->setFixedHeight(116);
    wordFrame->setStyleSheet(QString(
                                 "background:%1; border:1px solid %2; border-radius:16px;"
                                 ).arg(cardBg, border));

    auto *wfl = new QVBoxLayout(wordFrame);
    wfl->setContentsMargins(20, 10, 20, 10);

    wordDisplay = new QLabel;
    wordDisplay->setWordWrap(true);
    wordDisplay->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    wordDisplay->setStyleSheet(
        "font-family:'Courier New',monospace; font-size:19px;"
        "line-height:1.7; background:transparent; border:none;"
        );
    wfl->addWidget(wordDisplay);

    // ── Input Box ─────────────────────────────────────────
    inputBox = new QLineEdit;
    inputBox->setPlaceholderText(isEnglish ? "Start typing here..." : "Mulai mengetik di sini...");
    inputBox->setFixedHeight(60);
    inputBox->setStyleSheet(QString(
                                "QLineEdit { border:2px solid %1; border-radius:14px;"
                                "  padding-left:18px; font-size:17px;"
                                "  font-family:'Courier New',monospace;"
                                "  background:%2; color:%3; }"
                                "QLineEdit:focus { border-color:%4; }"
                                ).arg(border, cardBg, textMain, isDarkMode ? "#6366F1" : "#60A5FA"));

    auto styleInput = [this](bool wrong) {
        QString bg      = isDarkMode ? "#1E2130" : "white";
        QString txtCol  = isDarkMode ? "#E2E8F0" : "#111827";
        QString focusBorder = isDarkMode ? "#6366F1" : "#60A5FA";
        if (wrong) {
            inputBox->setStyleSheet(
                "QLineEdit { border:2px solid #EF4444; border-radius:14px;"
                "  padding-left:18px; font-size:17px;"
                "  font-family:'Courier New',monospace;"
                "  background:#2D1515; color:#FCA5A5; }"
                );
        } else {
            inputBox->setStyleSheet(QString(
                                        "QLineEdit { border:2px solid %1; border-radius:14px;"
                                        "  padding-left:18px; font-size:17px;"
                                        "  font-family:'Courier New',monospace;"
                                        "  background:%2; color:%3; }"
                                        ).arg(focusBorder, bg, txtCol));
        }
    };

    // ── Reset Button ──────────────────────────────────────
    QString resetTxt = isEnglish ? "↺  Reset Test" : "↺  Reset Tes";
    auto *resetBtn = new QPushButton(resetTxt);
    resetBtn->setFixedSize(150, 42);
    resetBtn->setStyleSheet(QString(
                                "QPushButton { background:%1; border:1px solid %2; border-radius:20px;"
                                "  font-size:13px; font-weight:600; color:%3; }"
                                "QPushButton:hover { background:%4; }"
                                ).arg(cardBg, border, textMain, isDarkMode ? "#2E3350" : "#F3F4F6"));

    // ── Layout ────────────────────────────────────────────
    root->addLayout(navbar);
    root->addSpacing(14);
    root->addWidget(badge, 0, Qt::AlignCenter);
    root->addSpacing(18);
    root->addLayout(statsRow);
    root->addSpacing(16);
    root->addWidget(wordFrame);
    root->addSpacing(12);
    root->addWidget(inputBox);
    root->addSpacing(18);
    root->addWidget(resetBtn, 0, Qt::AlignCenter);
    root->addStretch();

    // ══════════════════════════════════════════════════════
    // TIMER — Hitung mundur 1 detik
    // ══════════════════════════════════════════════════════

    countdownTimer = new QTimer(page);
    connect(countdownTimer, &QTimer::timeout, this, [this]() {
        timeLeft--;
        if (timerLabel) timerLabel->setText(QString::number(timeLeft));

        if (timeLeft <= 0) {
            countdownTimer->stop();
            inputBox->setEnabled(false);

            double minutes = testDuration / 60.0;
            int    finalWpm = (minutes > 0) ? qRound(currentWordIndex / minutes) : 0;

            int    total    = correctChars + incorrectChars;
            double finalAcc = (total > 0) ? (correctChars * 100.0 / total) : 100.0;

            if (finalWpm > highestWPM)      highestWPM      = finalWpm;
            if (finalAcc > highestAccuracy) highestAccuracy = finalAcc;
            totalTests++;

            if (isLoggedIn) saveUserStats(currentNIM);

            showResultPage(finalWpm, finalAcc, testDuration,
                           correctChars, incorrectChars, total);
        }
    });

    // ══════════════════════════════════════════════════════
    // INPUT LOGIC — Proses setiap karakter yang diketik
    // ══════════════════════════════════════════════════════

    connect(inputBox, &QLineEdit::textChanged, this, [this, styleInput](const QString &text) {

        if (!testStarted && !text.isEmpty()) {
            testStarted = true;
            countdownTimer->start(1000);
        }

        if (!testStarted || timeLeft <= 0) return;

        if (text.endsWith(' ')) {
            QString typed = text.trimmed();
            if (typed.isEmpty()) { inputBox->clear(); return; }

            const QString &target = wordList[currentWordIndex];
            int len = qMax(typed.length(), target.length());

            for (int i = 0; i < len; ++i) {
                bool ok = (i < typed.length() && i < target.length())
                ? (typed[i] == target[i]) : false;
                if (ok) correctChars++;
                else    incorrectChars++;
            }

            currentWordIndex++;
            inputBox->clear();
            refreshWordDisplay();

            int elapsed = testDuration - timeLeft;
            if (elapsed > 0) {
                int wpm = qRound(currentWordIndex / (elapsed / 60.0));
                if (wpmLabel) wpmLabel->setText(QString::number(wpm));
            }

            int    tot = correctChars + incorrectChars;
            double acc = (tot > 0) ? (correctChars * 100.0 / tot) : 100.0;
            if (accuracyLabel) accuracyLabel->setText(QString::number(qRound(acc)));

            styleInput(false);
            return;
        }

        if (!text.isEmpty()) {
            const QString &target = (currentWordIndex < wordList.size())
            ? wordList[currentWordIndex] : "";

            int tmpCor = 0, tmpInc = 0;
            for (int i = 0; i < text.length(); ++i) {
                if (i < target.length()) {
                    (text[i] == target[i]) ? tmpCor++ : tmpInc++;
                } else {
                    tmpInc++;
                }
            }

            int    tot = (correctChars + incorrectChars) + (tmpCor + tmpInc);
            int    cor = correctChars + tmpCor;
            double acc = (tot > 0) ? (cor * 100.0 / tot) : 100.0;
            if (accuracyLabel) accuracyLabel->setText(QString::number(qRound(acc)));

            bool wrong = false;
            for (int i = 0; i < text.length(); ++i) {
                if (i >= target.length() || text[i] != target[i]) {
                    wrong = true; break;
                }
            }
            styleInput(wrong);
        }
    });

    // ══════════════════════════════════════════════════════
    // RESET LOGIC
    // ══════════════════════════════════════════════════════

    connect(resetBtn, &QPushButton::clicked, this, [this, styleInput, cardBg, border, textMain]() {
        if (countdownTimer) countdownTimer->stop();
        if (isLoggedIn && !currentNIM.isEmpty()) saveUserStats(currentNIM);

        testStarted      = false;
        currentWordIndex = 0;
        correctChars     = 0;
        incorrectChars   = 0;
        timeLeft         = testDuration;

        wordList = generateWordList(testDuration);

        if (wpmLabel)      wpmLabel->setText("0");
        if (accuracyLabel) accuracyLabel->setText("100");
        if (timerLabel)    timerLabel->setText(QString::number(testDuration));

        inputBox->setEnabled(true);
        inputBox->clear();
        inputBox->setStyleSheet(QString(
                                    "QLineEdit { border:2px solid %1; border-radius:14px;"
                                    "  padding-left:18px; font-size:17px;"
                                    "  font-family:'Courier New',monospace;"
                                    "  background:%2; color:%3; }"
                                    "QLineEdit:focus { border-color:%4; }"
                                    ).arg(border, cardBg, textMain, isDarkMode ? "#6366F1" : "#60A5FA"));

        refreshWordDisplay();
        inputBox->setFocus();
    });

    QTimer::singleShot(80, this, [this]() {
        if (inputBox) inputBox->setFocus();
    });

    return page;
}

// ======================================================
// TYPING TEST — Inisialisasi sesi baru
// ======================================================

void MainWindow::runTypingTest(int seconds)
{
    testDuration     = seconds;
    timeLeft         = seconds;
    testStarted      = false;
    currentWordIndex = 0;
    correctChars     = 0;
    incorrectChars   = 0;

    wordList = generateWordList(seconds);

    if (typingPage) {
        stackedWidget->removeWidget(typingPage);
        typingPage->deleteLater();
        typingPage = nullptr;
    }

    typingPage = createTypingPage(seconds);
    stackedWidget->addWidget(typingPage);
    stackedWidget->setCurrentWidget(typingPage);

    refreshWordDisplay();
}

// ======================================================
// RESULT PAGE — Halaman setelah tes selesai
// ======================================================

void MainWindow::showResultPage(int wpm, double accuracy, int duration,
                                int corrChars, int incorrChars, int totalChars)
{
    if (resultPage) {
        stackedWidget->removeWidget(resultPage);
        resultPage->deleteLater();
        resultPage = nullptr;
    }

    QString pageBg   = isDarkMode ? "#0F1117" : "#F5F7FB";
    QString cardBg   = isDarkMode ? "#1E2130" : "white";
    QString border   = isDarkMode ? "#2E3350" : "#E5E7EB";
    QString textMain = isDarkMode ? "#E2E8F0" : "#111827";
    QString textSub  = isDarkMode ? "#94A3B8" : "#6B7280";

    QWidget *page = new QWidget;
    page->setStyleSheet(QString("background-color:%1;").arg(pageBg));

    auto *root = new QVBoxLayout(page);
    root->setContentsMargins(0, 0, 0, 0);

    auto *scroll = new QScrollArea;
    scroll->setWidgetResizable(true);
    scroll->setFrameShape(QFrame::NoFrame);
    scroll->setStyleSheet("background:transparent; border:none;");

    auto *content = new QWidget;
    content->setStyleSheet("background:transparent;");
    auto *cl = new QVBoxLayout(content);
    cl->setAlignment(Qt::AlignHCenter | Qt::AlignTop);
    cl->setContentsMargins(40, 40, 40, 40);
    cl->setSpacing(0);

    // ── Trophy & Judul ────────────────────────────────────
    auto *trophy = new QLabel("🏆");
    trophy->setAlignment(Qt::AlignCenter);
    trophy->setStyleSheet("font-size:60px; background:transparent; border:none;");

    auto *titleLbl = new QLabel(isEnglish ? "Test Complete!" : "Tes Selesai!");
    titleLbl->setAlignment(Qt::AlignCenter);
    titleLbl->setStyleSheet(QString(
                                "font-size:34px; font-weight:800; color:%1;"
                                " background:transparent; border:none;"
                                ).arg(textMain));

    QString motiv;
    if      (wpm >= 80) motiv = isEnglish ? "Outstanding! 🚀"      : "Luar biasa! 🚀";
    else if (wpm >= 50) motiv = isEnglish ? "Great job! 💪"         : "Bagus sekali! 💪";
    else if (wpm >= 30) motiv = isEnglish ? "Keep practicing! 🔥"   : "Terus berlatih! 🔥";
    else                motiv = isEnglish ? "Keep going! 💡"         : "Semangat! Latihan lagi! 💡";

    auto *motivLbl = new QLabel(motiv);
    motivLbl->setAlignment(Qt::AlignCenter);
    motivLbl->setStyleSheet("font-size:17px; font-weight:600; color:#EA580C;"
                            " background:transparent; border:none;");

    cl->addWidget(trophy);
    cl->addWidget(titleLbl);
    cl->addSpacing(4);
    cl->addWidget(motivLbl);
    cl->addSpacing(28);

    // ── WPM & Accuracy Cards ──────────────────────────────
    auto makeResultCard = [](const QString &icon, const QString &label,
                             const QString &value, const QString &unit,
                             const QString &bg,    const QString &bdr,
                             const QString &color) -> QFrame*
    {
        auto *card = new QFrame;
        card->setFixedSize(210, 170);
        card->setStyleSheet(QString(
                                "background:%1; border:1px solid %2; border-radius:20px;"
                                ).arg(bg, bdr));
        auto *vl = new QVBoxLayout(card);
        vl->setAlignment(Qt::AlignCenter);
        vl->setSpacing(3);
        auto mk = [&](const QString &t, int sz, bool bold) {
            auto *l = new QLabel(t);
            l->setAlignment(Qt::AlignCenter);
            l->setStyleSheet(QString(
                                 "font-size:%1px; font-weight:%2; color:%3;"
                                 " background:transparent; border:none;"
                                 ).arg(sz).arg(bold?"800":"600").arg(color));
            return l;
        };
        vl->addWidget(mk(icon,  26, false));
        vl->addWidget(mk(label, 13, true));
        vl->addWidget(mk(value, 50, true));
        vl->addWidget(mk(unit,  13, true));
        return card;
    };

    QString speedBg = isDarkMode ? "#1a2440" : "#EFF6FF";
    QString accBg   = isDarkMode ? "#162A1E" : "#F0FDF4";

    auto *topCards = new QHBoxLayout;
    topCards->setSpacing(18);
    topCards->setAlignment(Qt::AlignCenter);
    topCards->addWidget(makeResultCard("🎯","Speed",    QString::number(wpm),           "WPM",
                                       speedBg,"#BFDBFE","#60A5FA"));
    topCards->addWidget(makeResultCard("✅","Accuracy", QString::number(qRound(accuracy)),"%",
                                       accBg,  "#BBF7D0","#4ADE80"));
    cl->addLayout(topCards);
    cl->addSpacing(24);

    // ── Statistik Lengkap ─────────────────────────────────
    auto *statsFrame = new QFrame;
    statsFrame->setFixedWidth(480);
    statsFrame->setStyleSheet(QString(
                                  "background:%1; border:1px solid %2; border-radius:18px;"
                                  ).arg(cardBg, border));

    auto *sfl = new QVBoxLayout(statsFrame);
    sfl->setContentsMargins(24, 18, 24, 18);
    sfl->setSpacing(0);

    auto *stTitle = new QLabel(isEnglish ? "Detailed Statistics" : "Statistik Lengkap");
    stTitle->setStyleSheet(QString(
                               "font-size:16px; font-weight:700; color:%1;"
                               " background:transparent; border:none;"
                               ).arg(textMain));
    sfl->addWidget(stTitle);
    sfl->addSpacing(14);

    auto addRow = [&](const QString &icon, const QString &label,
                      const QString &value, const QString &color, bool sep = true)
    {
        auto *row = new QHBoxLayout;
        auto *lbl = new QLabel(icon + "  " + label);
        lbl->setStyleSheet(QString(
                               "font-size:14px; color:%1; background:transparent; border:none;"
                               ).arg(textSub));
        auto *val = new QLabel(value);
        val->setStyleSheet(QString(
                               "font-size:14px; font-weight:700; color:%1;"
                               " background:transparent; border:none;"
                               ).arg(color));
        row->addWidget(lbl); row->addStretch(); row->addWidget(val);
        sfl->addLayout(row);
        if (sep) {
            sfl->addSpacing(8);
            auto *line = new QFrame;
            line->setFrameShape(QFrame::HLine);
            line->setStyleSheet(QString("color:%1; background:%1;").arg(border));
            sfl->addWidget(line);
            sfl->addSpacing(8);
        }
    };

    addRow("🕐", isEnglish?"Time Elapsed":"Waktu Berlalu",
           isEnglish ? QString("%1 sec").arg(duration) : QString("%1 detik").arg(duration),
           textMain);
    addRow("✅", isEnglish?"Correct Chars":"Karakter Benar",
           QString::number(corrChars),  "#16A34A");
    addRow("❌", isEnglish?"Incorrect Chars":"Karakter Salah",
           QString::number(incorrChars), "#DC2626");
    addRow("📊", isEnglish?"Total Chars":"Total Karakter",
           QString::number(totalChars),  textMain, false);

    auto *sfWrap = new QHBoxLayout;
    sfWrap->setAlignment(Qt::AlignCenter);
    sfWrap->addWidget(statsFrame);
    cl->addLayout(sfWrap);
    cl->addSpacing(28);

    // ── Tombol ────────────────────────────────────────────
    auto *btnRow = new QHBoxLayout;
    btnRow->setAlignment(Qt::AlignCenter);
    btnRow->setSpacing(12);

    QString btnBg  = isDarkMode ? "#6366F1" : "#05052E";
    QString btnHov = isDarkMode ? "#4F46E5" : "#11114A";

    auto *retryBtn = new QPushButton(isEnglish ? "↺  Try Again" : "↺  Coba Lagi");
    retryBtn->setFixedSize(190, 50);
    retryBtn->setStyleSheet(QString(
                                "QPushButton { background:%1; color:white; font-size:15px;"
                                "  font-weight:700; border-radius:14px; border:none; }"
                                "QPushButton:hover { background:%2; }"
                                ).arg(btnBg, btnHov));

    auto *homeBtn = new QPushButton(isEnglish ? "🏠  Back to Home" : "🏠  Kembali ke Beranda");
    homeBtn->setFixedSize(210, 50);
    homeBtn->setStyleSheet(QString(
                               "QPushButton { background:%1; color:%2; font-size:15px;"
                               "  font-weight:600; border-radius:14px; border:1px solid %3; }"
                               "QPushButton:hover { background:%4; }"
                               ).arg(cardBg, textMain, border, isDarkMode?"#2E3350":"#F3F4F6"));

    connect(retryBtn, &QPushButton::clicked, this, [this]() { runTypingTest(testDuration); });
    connect(homeBtn,  &QPushButton::clicked, this, [this]() {
        stackedWidget->setCurrentWidget(homePage);
    });

    btnRow->addWidget(retryBtn);
    btnRow->addWidget(homeBtn);
    cl->addLayout(btnRow);
    cl->addSpacing(20);

    scroll->setWidget(content);
    root->addWidget(scroll);

    resultPage = page;
    stackedWidget->addWidget(resultPage);
    stackedWidget->setCurrentWidget(resultPage);
}

// ======================================================
// LOGIN POPUP
// ======================================================

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

    // ── Close ─────────────────────────────────────────────
    auto *topRow = new QHBoxLayout;
    auto *closeBtn = new QPushButton("✕");
    closeBtn->setFixedSize(34, 34);
    closeBtn->setStyleSheet(QString(
                                "QPushButton { background:transparent; border:none; font-size:18px;"
                                "  font-weight:bold; color:%1; }"
                                "QPushButton:hover { color:%2; background:%3; border-radius:9px; }"
                                ).arg(textSub, textMain, isDarkMode?"#2E3350":"#F3F4F6"));
    connect(closeBtn, &QPushButton::clicked, &dlg, &QDialog::reject);
    topRow->addStretch();
    topRow->addWidget(closeBtn);
    vl->addLayout(topRow);

    // ── Judul ─────────────────────────────────────────────
    auto *ttl = new QLabel("Login");
    ttl->setAlignment(Qt::AlignCenter);
    ttl->setStyleSheet("font-size:30px; font-weight:800;");

    auto *sub = new QLabel(isEnglish ? "Please enter your Student ID" : "Silahkan masukkan NIM anda");
    sub->setAlignment(Qt::AlignCenter);
    sub->setStyleSheet(QString("font-size:14px; color:%1;").arg(textSub));

    // ── Input NIM ─────────────────────────────────────────
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
                                     isDarkMode?"#6366F1":"#60A5FA",
                                     isDarkMode?"#252840":"white"));
    QTimer::singleShot(0, nimInput, [nimInput]() { nimInput->setFocus(); });

    // ── Status ────────────────────────────────────────────
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
            statusLbl->setText(isEnglish ? "✔  Welcome back!" : "✔  Selamat datang kembali!");
        } else {
            statusLbl->setStyleSheet("font-size:12px; color:#60A5FA;");
            statusLbl->setText(isEnglish ? "✦  New account will be created" : "✦  Akun baru akan dibuat");
        }
    });

    // ── Tombol Login ──────────────────────────────────────
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
                                 isEnglish ? "Student ID cannot be empty!" : "NIM tidak boleh kosong!");
            return;
        }

        // ── Validasi: harus ada di daftar kelas ──────────
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

// ======================================================
// PROFILE POPUP
// ======================================================

void MainWindow::showProfilePopup()
{
    QString cardBg   = isDarkMode ? "#1E2130" : "#FFFFFF";
    QString border   = isDarkMode ? "#2E3350" : "#E5E7EB";
    QString textMain = isDarkMode ? "#E2E8F0" : "#111827";
    QString textSub  = isDarkMode ? "#94A3B8" : "#6B7280";

    QGraphicsBlurEffect *blur = new QGraphicsBlurEffect;
    blur->setBlurRadius(10);
    stackedWidget->setGraphicsEffect(blur);

    QDialog dlg(this);
    dlg.setModal(true);
    dlg.setFixedSize(460, 450);
    dlg.setWindowFlags(Qt::FramelessWindowHint);
    dlg.setStyleSheet(QString(
                          "QDialog { background:%1; border-radius:26px; border:1px solid %2; }"
                          "QLabel  { color:%3; background:transparent; border:none; }"
                          ).arg(cardBg, border, textMain));

    auto *vl = new QVBoxLayout(&dlg);
    vl->setContentsMargins(28, 14, 28, 26);
    vl->setSpacing(0);

    // ── Close ─────────────────────────────────────────────
    auto *topRow = new QHBoxLayout;
    auto *closeBtn = new QPushButton("✕");
    closeBtn->setFixedSize(34, 34);
    closeBtn->setStyleSheet(QString(
                                "QPushButton { background:transparent; border:none; font-size:17px;"
                                "  font-weight:bold; color:%1; }"
                                "QPushButton:hover { color:%2; background:%3; border-radius:9px; }"
                                ).arg(textSub, textMain, isDarkMode?"#2E3350":"#F3F4F6"));
    connect(closeBtn, &QPushButton::clicked, &dlg, &QDialog::reject);
    topRow->addStretch();
    topRow->addWidget(closeBtn);
    vl->addLayout(topRow);

    // ── Judul ─────────────────────────────────────────────
    auto *titleLbl = new QLabel("PROFILE");
    titleLbl->setAlignment(Qt::AlignCenter);
    titleLbl->setStyleSheet("font-size:22px; font-weight:800; letter-spacing:2px;");
    vl->addWidget(titleLbl);
    vl->addSpacing(16);

    // ── Avatar ────────────────────────────────────────────
    auto *avatar = new QLabel("👤");
    avatar->setFixedSize(76, 76);
    avatar->setAlignment(Qt::AlignCenter);
    avatar->setStyleSheet(QString(
                              "background:%1; border-radius:38px; font-size:34px; border:3px solid %2;"
                              ).arg(isDarkMode?"#1E2D5A":"#EEF2FF",
                                   isDarkMode?"#6366F1":"#C7D2FE"));
    vl->addWidget(avatar, 0, Qt::AlignCenter);
    vl->addSpacing(10);

    // ── Nama Lengkap ──────────────────────────────────────
    auto *nameLbl = new QLabel(currentName);
    nameLbl->setAlignment(Qt::AlignCenter);
    nameLbl->setStyleSheet("font-size:17px; font-weight:800;");
    vl->addWidget(nameLbl);
    vl->addSpacing(2);

    // ── NIM ───────────────────────────────────────────────
    auto *nimLbl = new QLabel("NIM: " + currentNIM);
    nimLbl->setAlignment(Qt::AlignCenter);
    nimLbl->setStyleSheet(QString("font-size:13px; color:%1;").arg(textSub));
    vl->addWidget(nimLbl);
    vl->addSpacing(3);

    auto *memLbl = new QLabel("Member Eceknya Type");
    memLbl->setAlignment(Qt::AlignCenter);
    memLbl->setStyleSheet(QString("font-size:12px; color:%1;").arg(textSub));
    vl->addWidget(memLbl);
    vl->addSpacing(20);

    // ── Stat Cards ────────────────────────────────────────
    auto makeCard = [&](const QString &value, const QString &label,
                        const QString &lightBg, const QString &bdr,
                        const QString &color) -> QFrame*
    {
        auto *card = new QFrame;
        card->setFixedHeight(96);
        card->setStyleSheet(QString(
                                "background:%1; border:2px solid %2; border-radius:16px;"
                                ).arg(isDarkMode ? cardBg : lightBg, bdr));
        auto *cl = new QVBoxLayout(card);
        cl->setAlignment(Qt::AlignCenter);
        cl->setSpacing(2);
        auto *val = new QLabel(value);
        val->setAlignment(Qt::AlignCenter);
        val->setStyleSheet(QString(
                               "font-size:28px; font-weight:800; color:%1;"
                               " background:transparent; border:none;"
                               ).arg(color));
        auto *lbl = new QLabel(label);
        lbl->setAlignment(Qt::AlignCenter);
        lbl->setStyleSheet(QString(
                               "font-size:12px; font-weight:600; color:%1;"
                               " background:transparent; border:none;"
                               ).arg(textSub));
        cl->addWidget(val);
        cl->addWidget(lbl);
        return card;
    };

    QString wpmVal  = highestWPM > 0       ? QString::number(highestWPM)                   : "—";
    QString accVal  = highestAccuracy > 0.0 ? QString::number(highestAccuracy,'f',1) + "%" : "—";
    QString testVal = totalTests > 0        ? QString::number(totalTests)                   : "—";

    auto *statsRow = new QHBoxLayout;
    statsRow->setSpacing(12);
    statsRow->addWidget(makeCard(wpmVal,  "Highest WPM",   "#EFF6FF","#60A5FA","#60A5FA"));
    statsRow->addWidget(makeCard(accVal,  "Best Accuracy", "#F0FDF4","#4ADE80","#4ADE80"));
    statsRow->addWidget(makeCard(testVal, "Total Tests",   "#FFF7ED","#FCA5A5","#FB923C"));
    vl->addLayout(statsRow);
    vl->addSpacing(20);

    // ── Logout ────────────────────────────────────────────
    auto *logoutBtn = new QPushButton(isEnglish ? "Logout" : "Logout");
    logoutBtn->setFixedHeight(48);
    logoutBtn->setStyleSheet(
        "QPushButton { background:#FEF2F2; color:#DC2626; font-size:15px;"
        "  font-weight:700; border-radius:12px; border:2px solid #FECACA; }"
        "QPushButton:hover { background:#FEE2E2; border-color:#F87171; }"
        );
    connect(logoutBtn, &QPushButton::clicked, this, [&]() {
        if (!currentNIM.isEmpty()) saveUserStats(currentNIM);
        isLoggedIn = false; currentNIM = ""; currentName = "";
        highestWPM = 0; highestAccuracy = 0.0; totalTests = 0;
        if (loginProfileBtn) loginProfileBtn->setText("👤  Login");
        dlg.accept();
    });
    vl->addWidget(logoutBtn);

    dlg.exec();
    stackedWidget->setGraphicsEffect(nullptr);
}

// ======================================================
// SLOT WRAPPERS
// ======================================================

void MainWindow::showDurationPage() { stackedWidget->setCurrentWidget(durationPage); }