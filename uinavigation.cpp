#include "MainWindow.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QFrame>
#include <QLineEdit>
#include <QTimer>
#include <QRandomGenerator>

void MainWindow::rebuildMainPages()
{
    QWidget *prev      = stackedWidget->currentWidget();
    bool wasOnDuration = (prev == durationPage);
    bool stayOnOther   = (prev != homePage && prev != durationPage);

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

void MainWindow::toggleTheme()
{
    isDarkMode = !isDarkMode;
    rebuildMainPages();
    refreshWordDisplay();
}

void MainWindow::toggleLanguage()
{
    isEnglish = !isEnglish;
    rebuildMainPages();
}

QWidget *MainWindow::createHomePage()
{
    QWidget *page = new QWidget;
    page->setStyleSheet(
        isDarkMode ? "background-color:#0F1117;" : "background-color:#F5F7FB;"
    );

    QVBoxLayout *root = new QVBoxLayout(page);
    root->setContentsMargins(40, 20, 40, 20);

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

    struct Feature { QString icon, title, desc; };
    QList<Feature> features = {
        { "🌐",
        isEnglish ? "Multi Language"  : "Multiple Bahasa",
        isEnglish ? "Hundreds of words\nin English & Indonesian"
                    : "Ratusan kata bahasa\nIndonesia & Inggris yang beragam" },
        { "📊",
        isEnglish ? "Full Statistics" : "Statistik Lengkap",
        isEnglish ? "Track your WPM\nand accuracy progress"
                    : "Pantau progres WPM\ndan akurasi kamu" },
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

QWidget *MainWindow::createDurationPage()
{
    QWidget *page = new QWidget;
    page->setStyleSheet(
        isDarkMode ? "background-color:#0F1117;" : "background-color:#F5F7FB;"
    );

    QVBoxLayout *root = new QVBoxLayout(page);
    root->setContentsMargins(40, 20, 40, 20);

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

    struct DurationCard {
        QString sec, icon;
        QString bgLight, borderLight, hoverLight;
        QString bgDark,  borderDark,  hoverDark;
    };
    QList<DurationCard> durations = {
        { "15", "⚡", "#FFFBEB","#FACC15","#FEF9C3", "#1E2130","#4B4010","#2A2A10" },
        { "30", "🎯", "#EFF6FF","#60A5FA","#DBEAFE", "#1E2130","#1E3A5F","#162540" },
        { "60", "🔥", "#FAF5FF","#D8B4FE","#F3E8FF", "#1E2130","#3B2060","#2A1545" },
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
        QString bg          = isDarkMode ? "#1E2130" : "white";
        QString txtCol      = isDarkMode ? "#E2E8F0" : "#111827";
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

    QString resetTxt = isEnglish ? "↺  Reset Test" : "↺  Reset Tes";
    auto *resetBtn = new QPushButton(resetTxt);
    resetBtn->setFixedSize(150, 42);
    resetBtn->setStyleSheet(QString(
        "QPushButton { background:%1; border:1px solid %2; border-radius:20px;"
        "  font-size:13px; font-weight:600; color:%3; }"
        "QPushButton:hover { background:%4; }"
    ).arg(cardBg, border, textMain, isDarkMode ? "#2E3350" : "#F3F4F6"));

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

    QTimer::singleShot(80, this, [this]() {
        if (inputBox) inputBox->setFocus();
    });

    return page;
}
