#include "MainWindow.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QFrame>
#include <QScrollArea>
#include <QDialog>
#include <QGraphicsBlurEffect>

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
    if      (wpm >= 80) motiv = isEnglish ? "Outstanding! 🚀"    : "Luar biasa! 🚀";
    else if (wpm >= 50) motiv = isEnglish ? "Great job! 💪"       : "Bagus sekali! 💪";
    else if (wpm >= 30) motiv = isEnglish ? "Keep practicing! 🔥" : "Terus berlatih! 🔥";
    else                motiv = isEnglish ? "Keep going! 💡"       : "Semangat! Latihan lagi! 💡";

    auto *motivLbl = new QLabel(motiv);
    motivLbl->setAlignment(Qt::AlignCenter);
    motivLbl->setStyleSheet(
        "font-size:17px; font-weight:600; color:#EA580C;"
        " background:transparent; border:none;"
    );

    cl->addWidget(trophy);
    cl->addWidget(titleLbl);
    cl->addSpacing(4);
    cl->addWidget(motivLbl);
    cl->addSpacing(28);

    // ── Kartu WPM & Akurasi ───────────────────────────────
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
            ).arg(sz).arg(bold ? "800" : "600").arg(color));
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
    topCards->addWidget(makeResultCard("🎯", "Speed",
                                        QString::number(wpm), "WPM",
                                        speedBg, "#BFDBFE", "#60A5FA"));
    topCards->addWidget(makeResultCard("✅", "Accuracy",
                                        QString::number(qRound(accuracy)), "%",
                                        accBg, "#BBF7D0", "#4ADE80"));
    cl->addLayout(topCards);
    cl->addSpacing(24);

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
        row->addWidget(lbl);
        row->addStretch();
        row->addWidget(val);
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

    addRow("🕐", isEnglish ? "Time Elapsed"    : "Waktu Berlalu",
        isEnglish ? QString("%1 sec").arg(duration) : QString("%1 detik").arg(duration),
        textMain);
    addRow("✅", isEnglish ? "Correct Chars"   : "Karakter Benar",
        QString::number(corrChars),   "#16A34A");
    addRow("❌", isEnglish ? "Incorrect Chars" : "Karakter Salah",
        QString::number(incorrChars), "#DC2626");
    addRow("📊", isEnglish ? "Total Chars"     : "Total Karakter",
        QString::number(totalChars),  textMain, false);

    auto *sfWrap = new QHBoxLayout;
    sfWrap->setAlignment(Qt::AlignCenter);
    sfWrap->addWidget(statsFrame);
    cl->addLayout(sfWrap);
    cl->addSpacing(28);

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
    ).arg(cardBg, textMain, border, isDarkMode ? "#2E3350" : "#F3F4F6"));

    connect(retryBtn, &QPushButton::clicked, this, [this]() {
        runTypingTest(testDuration);
    });

    connect(homeBtn, &QPushButton::clicked, this, [this]() {
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

    auto *topRow  = new QHBoxLayout;
    auto *closeBtn = new QPushButton("✕");
    closeBtn->setFixedSize(34, 34);
    closeBtn->setStyleSheet(QString(
        "QPushButton { background:transparent; border:none; font-size:17px;"
        "  font-weight:bold; color:%1; }"
        "QPushButton:hover { color:%2; background:%3; border-radius:9px; }"
    ).arg(textSub, textMain, isDarkMode ? "#2E3350" : "#F3F4F6"));
    connect(closeBtn, &QPushButton::clicked, &dlg, &QDialog::reject);
    topRow->addStretch();
    topRow->addWidget(closeBtn);
    vl->addLayout(topRow);

    auto *titleLbl = new QLabel("PROFILE");
    titleLbl->setAlignment(Qt::AlignCenter);
    titleLbl->setStyleSheet("font-size:22px; font-weight:800; letter-spacing:2px;");
    vl->addWidget(titleLbl);
    vl->addSpacing(16);

    auto *avatar = new QLabel("👤");
    avatar->setFixedSize(76, 76);
    avatar->setAlignment(Qt::AlignCenter);
    avatar->setStyleSheet(QString(
        "background:%1; border-radius:38px; font-size:34px; border:3px solid %2;"
    ).arg(isDarkMode ? "#1E2D5A" : "#EEF2FF",
        isDarkMode ? "#6366F1" : "#C7D2FE"));
    vl->addWidget(avatar, 0, Qt::AlignCenter);
    vl->addSpacing(10);

    auto *nameLbl = new QLabel(currentName);
    nameLbl->setAlignment(Qt::AlignCenter);
    nameLbl->setStyleSheet("font-size:17px; font-weight:800;");
    vl->addWidget(nameLbl);
    vl->addSpacing(2);

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

    auto makeCard = [&](const QString &value, const QString &label,
                        const QString &lightBg, const QString &bdr,
                        const QString &color) -> QFrame*
    {
        auto *card = new QFrame;
        card->setFixedHeight(96);
        card->setStyleSheet(QString(
            "background:%1; border:2px solid %2; border-radius:16px;"
        ).arg(isDarkMode ? cardBg : lightBg, bdr));

        auto *cl2 = new QVBoxLayout(card);
        cl2->setAlignment(Qt::AlignCenter);
        cl2->setSpacing(2);

        auto *val = new QLabel(value);
        val->setAlignment(Qt::AlignCenter);
        val->setStyleSheet(QString(
            "font-size:28px; font-weight:800; color:%1;"
            " background:transparent; border:none;"
        ).arg(color));

        auto *lbl2 = new QLabel(label);
        lbl2->setAlignment(Qt::AlignCenter);
        lbl2->setStyleSheet(QString(
            "font-size:12px; font-weight:600; color:%1;"
            " background:transparent; border:none;"
        ).arg(textSub));

        cl2->addWidget(val);
        cl2->addWidget(lbl2);
        return card;
    };

    QString wpmVal  = highestWPM > 0        ? QString::number(highestWPM)                    : "—";
    QString accVal  = highestAccuracy > 0.0  ? QString::number(highestAccuracy, 'f', 1) + "%" : "—";
    QString testVal = totalTests > 0         ? QString::number(totalTests)                    : "—";

    auto *statsRow = new QHBoxLayout;
    statsRow->setSpacing(12);
    statsRow->addWidget(makeCard(wpmVal,  "Highest WPM",   "#EFF6FF", "#60A5FA", "#60A5FA"));
    statsRow->addWidget(makeCard(accVal,  "Best Accuracy", "#F0FDF4", "#4ADE80", "#4ADE80"));
    statsRow->addWidget(makeCard(testVal, "Total Tests",   "#FFF7ED", "#FCA5A5", "#FB923C"));
    vl->addLayout(statsRow);
    vl->addSpacing(20);

    auto *logoutBtn = new QPushButton("Logout");
    logoutBtn->setFixedHeight(48);
    logoutBtn->setStyleSheet(
        "QPushButton { background:#FEF2F2; color:#DC2626; font-size:15px;"
        "  font-weight:700; border-radius:12px; border:2px solid #FECACA; }"
        "QPushButton:hover { background:#FEE2E2; border-color:#F87171; }"
    );
    connect(logoutBtn, &QPushButton::clicked, this, [&]() {

        if (!currentNIM.isEmpty()) saveUserStats(currentNIM);

        isLoggedIn      = false;
        currentNIM      = "";
        currentName     = "";
        highestWPM      = 0;
        highestAccuracy = 0.0;
        totalTests      = 0;

        if (loginProfileBtn) loginProfileBtn->setText("👤  Login");
        dlg.accept();
    });
    vl->addWidget(logoutBtn);

    dlg.exec();

    stackedWidget->setGraphicsEffect(nullptr);
}

void MainWindow::showDurationPage()
{
    stackedWidget->setCurrentWidget(durationPage);
}