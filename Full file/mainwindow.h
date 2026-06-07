// ========================== MainWindow.h ==========================
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

// ── Qt Core ───────────────────────────────────────────
#include <QMainWindow>
#include <QStackedWidget>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QTimer>
#include <QList>
#include <QStringList>

// ── Qt SQL ────────────────────────────────────────────
#include <QSqlDatabase>

// ======================================================
// CLASS DECLARATION
// ======================================================

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    // ── Private Slots ─────────────────────────────────────
private slots:
    void showDurationPage();
    void showLoginPopup();
    void showProfilePopup();

    // ── Private Methods ───────────────────────────────────
private:

    // --- PAGE BUILDERS ---
    QWidget *createHomePage();
    QWidget *createDurationPage();
    QWidget *createTypingPage(int seconds);
    void     rebuildMainPages();

    // --- DATABASE ---
    void initDatabase();
    bool nimExists(const QString &nim);
    void registerNIM(const QString &nim);
    void loadUserStats(const QString &nim);
    void saveUserStats(const QString &nim);
    QString nameFromNIM(const QString &nim);   // lookup nama dari daftar kelas

    // --- POPUPS ---
    void showResultPage(int wpm, double accuracy, int duration,
                        int corrChars, int incorrChars, int totalChars);

    // --- TYPING TEST ---
    void runTypingTest(int seconds);
    void refreshWordDisplay();
    QStringList generateWordList(int seconds);

    // --- THEME & LANGUAGE ---
    void toggleTheme();
    void toggleLanguage();

    // ── Private Members ───────────────────────────────────
private:

    // --- PAGES ---
    QStackedWidget *stackedWidget = nullptr;
    QWidget        *homePage      = nullptr;
    QWidget        *durationPage  = nullptr;
    QWidget        *typingPage    = nullptr;
    QWidget        *resultPage    = nullptr;

    // --- TYPING TEST STATE ---
    QStringList wordList;
    int         currentWordIndex = 0;
    int         correctChars     = 0;
    int         incorrectChars   = 0;
    bool        testStarted      = false;
    int         timeLeft         = 0;
    int         testDuration     = 0;

    // --- TYPING PAGE WIDGETS (diakses lintas-fungsi) ---
    QLabel    *wpmLabel       = nullptr;
    QLabel    *accuracyLabel  = nullptr;
    QLabel    *timerLabel     = nullptr;
    QLabel    *wordDisplay    = nullptr;
    QLineEdit *inputBox       = nullptr;
    QTimer    *countdownTimer = nullptr;

    // --- DATABASE ---
    QSqlDatabase db;

    // --- USER / SESSION ---
    bool    isLoggedIn      = false;
    QString currentNIM      = "";
    QString currentName     = "";   // nama lengkap dari daftar kelas
    int     highestWPM      = 0;
    double  highestAccuracy = 0.0;
    int     totalTests      = 0;

    // --- NAVBAR ---
    QPushButton *loginProfileBtn = nullptr;
    QPushButton *themeBtn        = nullptr;
    QPushButton *langBtn         = nullptr;

    // --- SETTINGS ---
    bool isDarkMode = false;
    bool isEnglish  = false;
};

#endif // MAINWINDOW_H