#include "MainWindow.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QRandomGenerator>
#include <QTimer>

static const QStringList KATA_ID = {

    "aku","kamu","dia","kita","mereka","dan","atau","tapi","jika","karena",

    "sudah","belum","akan","bisa","mau","perlu","harus","boleh","tidak","ya",

    "rumah","sekolah","kantor","jalan","pasar","taman","kota","desa","laut","gunung",

    "makan","minum","tidur","belajar","kerja","lari","bicara","dengar","lihat","masak",

    "besar","kecil","tinggi","rendah","cepat","lambat","panas","dingin","baik","buruk",

    "satu","dua","tiga","empat","lima","enam","tujuh","delapan","sembilan","sepuluh",

    "hari","bulan","tahun","waktu","pagi","siang","sore","malam","minggu","detik",

    "air","api","tanah","udara","cahaya","suara","warna","bentuk","ukuran","berat",

    "orang","anak","ibu","ayah","kakak","adik","teman","guru","dokter","polisi",

    "buku","pensil","kertas","meja","kursi","pintu","jendela","lantai","atap","dinding",

    "kucing","anjing","burung","ikan","kuda","sapi","kambing","ayam","bebek","kelinci",

    "buah","sayur","nasi","roti","mie","sup","kue","gula","garam","minyak",

    "mobil","motor","sepeda","bus","kereta","kapal","pesawat","truk","becak","angkot",

    "senang","sedih","marah","takut","heran","malu","bangga","kecewa","lega","bosan",

    "pintar","rajin","malas","jujur","sopan","kasar","sabar","hemat","berani","rendah",

    "indah","baru","lama","jauh","dekat","dalam","tebal","tipis","mudah","susah",

    "aman","bersih","kotor","ramai","sepi","terang","gelap","sehat","sakit","segar",

    "mulai","selesai","buka","tutup","masuk","keluar","naik","turun","datang","pergi",

    "membaca","menulis","menyanyi","menari","memasak","membantu","menjaga","mencari",

    "harapan","impian","tujuan","rencana","usaha","hasil","manfaat","kepercayaan",

    "persahabatan","keluarga","cinta","kasih","sayang","perhatian","hormat","kerja",

    "komputer","telepon","internet","aplikasi","program","data","informasi","teknologi",

    "pelajaran","ujian","nilai","prestasi","juara","hadiah","penghargaan","sertifikat",

    "negara","bangsa","budaya","bahasa","sejarah","tradisi","adat","norma","hukum",

    "kesehatan","olahraga","istirahat","vitamin","gizi","energi","kekuatan","stamina",

    "masalah","solusi","pertanyaan","jawaban","pendapat","diskusi","argumen","keputusan",

    "perubahan","kemajuan","inovasi","kreativitas","ide","gagasan","konsep","strategi"
};

static const QStringList KATA_EN = {

    "the","a","an","and","or","but","if","because","when","while",

    "run","walk","eat","sleep","write","read","talk","look","make","work",

    "home","school","office","road","park","city","town","sea","mountain","forest",

    "big","small","tall","short","fast","slow","hot","cold","good","bad",

    "one","two","three","four","five","six","seven","eight","nine","ten",

    "day","month","year","time","morning","noon","evening","night","week","second",

    "water","fire","earth","air","light","sound","color","shape","size","weight",

    "person","child","mother","father","friend","teacher","doctor","student","brother","sister",

    "book","pen","paper","table","chair","door","window","floor","wall","roof",

    "cat","dog","bird","fish","horse","cow","goat","chicken","duck","rabbit",

    "fruit","rice","bread","soup","cake","sugar","salt","oil","noodle","vegetable",

    "car","bike","bus","train","ship","plane","truck","boat","taxi","van",

    "happy","sad","angry","scared","proud","bored","tired","excited","calm","worried",

    "code","data","app","web","system","server","network","program","software","device",

    "start","stop","open","close","enter","exit","push","pull","send","receive",

    "new","old","near","far","deep","thick","thin","easy","hard","clear",

    "hope","dream","goal","plan","idea","effort","result","trust","change","power",

    "lesson","test","score","study","learn","class","grade","exam","skill","knowledge",

    "family","love","care","team","group","share","help","support","respect","connect",

    "running","writing","reading","singing","dancing","cooking","helping",
    "watching","listening","thinking"
};

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

    QString cardBg   = isDarkMode ? "#1E2130" : "white";
    QString border   = isDarkMode ? "#2E3350" : "#E5E7EB";
    QString textMain = isDarkMode ? "#E2E8F0" : "#111827";

    QPushButton *resetBtn = typingPage->findChild<QPushButton *>("resetBtn");
    if (resetBtn) {
        connect(resetBtn, &QPushButton::clicked, this,
                [this, styleInput, cardBg, border, textMain]() {
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
    }
}
