// Copyright (c) 2016 The Zcash developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "uint256.h"

#include <atomic>
#include <mutex>
#include <string>

struct AtomicCounter {
    std::atomic<uint64_t> value;

    AtomicCounter() : value {0} { }

    void increment(){
        ++value;
    }

    void decrement(){
        --value;
    }

    int get() const {
        return value.load();
    }
};

class AtomicTimer {
private:
    std::mutex mtx;
    uint64_t threads;
    int64_t start_time;
    int64_t total_time;

public:
    AtomicTimer() : threads(0), start_time(0), total_time(0) {}

    /**
     * Starts timing on first call, and counts the number of calls.
     */
    void start();

    /**
     * Counts number of calls, and stops timing after it has been called as
     * many times as start().
     */
    void stop();

    bool running();

    uint64_t threadCount();

    double rate(const AtomicCounter& count);
};

extern AtomicCounter transactionsValidated;
extern AtomicCounter ehSolverRuns;
extern AtomicCounter solutionTargetChecks;
extern AtomicTimer miningTimer;

void TrackMinedBlock(uint256 hash);

void MarkStartTime();
double GetLocalSolPS();
int EstimateNetHeightInner(int height, int64_t tipmediantime,
                           int heightLastCheckpoint, int64_t timeLastCheckpoint,
                           int64_t genesisTime, int64_t targetSpacing);

void TriggerRefresh();

void ConnectMetricsScreen();
void ThreadShowMetricsScreen();

/**
 * Heart image: https://commons.wikimedia.org/wiki/File:Heart_coraz%C3%B3n.svg
 * License: CC BY-SA 3.0
 *
 * Rendering options:
 * bitcoinz: img2txt -W 80 -H 35 -f utf8 -d none -g bitcoinz.png > design.ini
 */
const std::string METRICS_ART =
"\n\nBitcoinZ - Your Financial Freedom\n\n"
""
"                            [0;36;5;40;100mttttttt[0;33;5;40;100mttttttttttttt[0;36;5;40;100mttttt[0m                           \n"
"                      [0;36;5;40;100mttttt[0;33;5;40;100mttt[0;36;5;40;100mtt%%%tt[0;33;5;40;100mtt%%%tt[0;36;5;40;100mt%%%%t[0;33;5;40;100mttttt[0;36;5;40;100mttt[0m                      \n"
"                  [0;36;5;40;100mt;ttt[0;33;5;40;100mt[0;36;5;40;100mt%%[0;35;5;40;100mt[0;33;5;40;100mtt[0;1;30;90;43m88[0;37;43m@@[0;37;5;43;103m@XXXXS[0;33;5;43;103m:::t%[0;1;33;93;43mXX%[0;1;30;90;43m88[0;33;5;40;100mXX%[0;36;5;40;100mt%[0;33;5;40;100mttt[0;36;5;40;100mt[0;33;5;40;100mt[0;36;5;40;100mttt[0m                 \n"
"               [0;36;5;40;100mtt;tt[0;33;5;40;100mttt[0;34;5;40;100mS[0;33;5;40;100mttt[0;33;47m8[0;37;5;43;103m8888@@@XXXSSSSS[0;33;5;43;103m.::::....:[0;1;30;90;43mS[0;33;5;40;100mS[0;34;5;40;100mSS[0;36;5;40;100m%[0;33;5;40;100mt[0;36;5;40;100mttt;t[0m                \n"
"              [0;36;5;40;100mt[0;33;5;40;100mtt[0;36;5;40;100mt[0;33;5;40;100mt[0;36;5;40;100mttt[0;33;5;40;100mt[0;33;47m8[0;37;5;43;103m888888888@[0;33;5;43;103m@[0;37;43m@@@[0;1;30;90;43m88888[0;37;43mX[0;1;33;93;43m%XX[0;33;5;43;103mt.......[0;1;33;93;43m%%[0;1;30;90;43m8[0;33;5;40;100mS[0;36;5;40;100m%tt[0;33;5;40;100mt[0;36;5;40;100mtttt[0m             \n"
"            [0;36;5;40;100mttt[0;33;5;40;100mtt[0;36;5;40;100m%[0;33;5;40;100mt:[0;1;33;93;47m88[0;37;5;43;103m@88888[0;33;47m8[0;1;30;90;43m8[0;33;5;40;100m%%t[0;35;5;40;100mt[0;36;5;40;100m%%%%%ttt%%%%%[0;33;5;40;100mt%X[0;1;30;90;43m88[0;1;33;93;43mt[0;33;5;43;103m;.....[0;1;33;93;43mt[0;1;30;90;43m88[0;33;5;40;100mt[0;36;5;40;100mt[0;33;5;40;100mttt[0;36;5;40;100mtt[0m           \n"
"         [0;36;5;40;100mtttt[0;33;5;40;100mt[0;36;5;40;100mt[0;33;5;40;100mtt[0;1;30;90;43m8[0;37;5;43;103m8X@@@8[0;33;47m8[0;33;5;40;100mttt[0;36;5;40;100m%t[0;33;5;40;100mtttt[0;36;5;40;100mttttttttttttt[0;33;5;40;100mttt[0;36;5;40;100mtt%[0;33;5;40;100mt8[0;1;30;90;43m@@[0;33;5;43;103m:....[0;1;33;93;43m%[0;1;30;90;43m8[0;36;5;40;100mttt[0;33;5;40;100mt[0;36;5;40;100mtt[0m          \n"
"        [0;36;5;40;100mt;tt[0;33;5;40;100mt[0;34;5;40;100m%[0;33;5;40;100m;[0;1;33;93;47m88[0;37;5;43;103mX@X[0;1;33;93;47m88[0;33;5;40;100m;t[0;36;5;40;100m%[0;33;5;40;100mttt[0;36;5;40;100mtttttt[0;33;5;40;100mt[0;36;5;40;100mtttttttttttttttttt[0;33;5;40;100mt[0;36;5;40;100mt%%[0;33;5;40;100mS[0;1;30;90;43m@[0;33;5;43;103m;....[0;1;33;93;43m;[0;33;5;40;100mXX[0;36;5;40;100m%[0;33;5;40;100mtt[0;36;5;40;100m;;t[0m       \n"
"       [0;36;5;40;100mt;ttt%[0;37;43m8[0;37;5;43;103m@@@X@[0;37;43m8[0;33;5;40;100mtt[0;36;5;40;100mt[0;33;5;40;100mt[0;36;5;40;100mtttttttttt[0;33;5;40;100mt[0;1;30;90;43m888[0;33;5;40;100mt[0;36;5;40;100mt[0;33;5;40;100mtt[0;36;5;40;100mtt[0;33;5;40;100mt[0;36;5;40;100mtttttttttttt[0;33;5;40;100mt[0;36;5;40;100m%[0;33;5;40;100mS[0;1;30;90;43mSS[0;33;5;43;103m...::[0;1;30;90;43m8[0;34;5;40;100m%[0;33;5;40;100mt[0;36;5;40;100mtt;t[0m      \n"
"     [0;36;5;40;100mtt;t[0;33;5;40;100mt[0;34;5;40;100mXX[0;33;5;40;100m;[0;37;5;43;103mX8@@X[0;33;5;40;100m;[0;34;5;40;100mX[0;33;5;40;100mtt[0;36;5;40;100mtttttttt[0;33;5;40;100mttt[0;36;5;40;100m%[0;37;43m@[0;37;5;43;103m@@X[0;33;5;40;100mS[0;34;5;40;100m@[0;36;5;40;100m%%[0;33;5;40;100mt[0;36;5;40;100m%[0;34;5;40;100mS[0;36;5;40;100mtt[0;33;5;40;100mt[0;36;5;40;100mtttttttttt[0;33;5;40;100mtt[0;34;5;40;100mSS[0;1;30;90;43mX[0;33;5;43;103m....:[0;33;5;40;100m@[0;34;5;40;100mS[0;33;5;40;100mtt[0;36;5;40;100mt;[0m      \n"
"     [0;36;5;40;100mttt[0;33;5;40;100mt[0;34;5;40;100mS[0;33;5;40;100mtt[0;1;33;93;47m8[0;37;5;43;103m@@@@[0;1;30;90;43m8[0;34;5;40;100m%[0;33;5;40;100mttt[0;36;5;40;100mtttttt%t[0;33;5;40;100mtttt[0;37;43m@[0;37;5;43;103m@@XX[0;1;33;93;43mX[0;33;5;43;103mSStS[0;37;43mS[0;33;5;40;100mXX[0;36;5;40;100mtt[0;33;5;40;100mt[0;36;5;40;100mttttttttt[0;33;5;40;100mt[0;36;5;40;100mtt[0;33;5;40;100mt[0;1;30;90;43mS[0;33;5;43;103m....[0;1;33;93;43mt[0;33;5;40;100m%[0;36;5;40;100mtt[0;33;5;40;100mt[0;36;5;40;100m;t[0m     \n"
"    [0;36;5;40;100mt;;[0;33;5;40;100mt[0;36;5;40;100mt[0;33;5;40;100mt[0;1;33;93;47m88[0;37;5;43;103m@8X[0;33;47m88[0;34;5;40;100mS[0;33;5;40;100mtt[0;36;5;40;100mtttt[0;33;5;40;100mt[0;34;5;40;100m%%[0;33;5;40;100mt[0;33;47m8[0;37;5;43;103m8888@@@@XXXSSS[0;33;5;43;103m....[0;37;43mS[0;33;5;40;100m%[0;36;5;40;100mt[0;33;5;40;100mtt[0;36;5;40;100mtttttttt[0;33;5;40;100mtt[0;36;5;40;100mt[0;33;5;40;100mt[0;1;33;93;43m;[0;33;5;43;103m....[0;1;30;90;43mS[0;36;5;40;100m%%[0;33;5;40;100mtt[0;36;5;40;100m;tt[0m   \n"
"    [0;36;5;40;100m;tt[0;33;5;40;100mt[0;34;5;40;100mS[0;1;30;90;43m8[0;37;5;43;103m@@8@[0;1;33;93;47m8[0;33;5;40;100mtt[0;36;5;40;100mt[0;33;5;40;100mt[0;36;5;40;100mttttt[0;33;5;40;100mt[0;34;5;40;100mSS[0;33;5;40;100mt[0;37;5;43;103m888888@[0;37;43m@@@[0;1;30;90;43m8[0;33;5;40;100mS[0;1;30;90;43m88[0;33;5;43;103mX[0;37;5;43;103mSS[0;33;5;43;103m...[0;1;30;90;43m8[0;34;5;40;100mS[0;33;5;40;100mtt[0;36;5;40;100mtttttttttt[0;33;5;40;100mt[0;36;5;40;100m%[0;33;5;40;100mX[0;33;5;43;103m;;..:[0;33;5;40;100m88[0;34;5;40;100mS[0;33;5;40;100mt[0;36;5;40;100mttt[0m   \n"
"   [0;36;5;40;100mt;[0;33;5;40;100mtt[0;36;5;40;100mt[0;33;5;40;100mt[0;1;33;93;47m8[0;37;5;43;103m@@8@[0;33;5;40;100m;[0;34;5;40;100mSS[0;33;5;40;100mt[0;36;5;40;100mtttttt[0;33;5;40;100mt[0;36;5;40;100mtt[0;33;5;40;100mt[0;33;47m8[0;37;5;43;103m88888[0;33;5;40;100m%[0;34;5;40;100mXX[0;36;5;40;100mtt[0;33;5;40;100mt[0;34;5;40;100mXX[0;33;5;40;100mS[0;37;5;43;103mXS[0;33;5;43;103m...[0;33;5;40;100mX[0;34;5;40;100mS[0;33;5;40;100mtt[0;36;5;40;100mttttttttttt[0;33;5;40;100mt[0;34;5;40;100mS[0;1;30;90;43m@@[0;33;5;43;103m...[0;1;30;90;43mXX[0;34;5;40;100mS[0;33;5;40;100mt[0;36;5;40;100mt;;[0m   \n"
"   [0;36;5;40;100mt;[0;33;5;40;100mtt[0;34;5;40;100m%[0;33;5;40;100mt[0;37;5;43;103m888@[0;1;33;93;47m8[0;33;5;40;100mt[0;34;5;40;100m%%[0;33;5;40;100mt[0;36;5;40;100mttttttt[0;33;5;40;100mtt[0;34;5;40;100mX[0;33;5;40;100mt[0;37;5;43;103m88888[0;33;47m8[0;33;5;40;100m%%%S[0;1;30;90;43m8[0;37;43m@@[0;37;5;43;103mXSS[0;33;5;43;103m::[0;1;30;90;43m8[0;34;5;40;100m%[0;36;5;40;100mt[0;33;5;40;100mtt[0;36;5;40;100mttttttttttt[0;33;5;40;100mt[0;34;5;40;100mS[0;33;5;40;100m88[0;33;5;43;103m...[0;1;33;93;43mtt[0;33;5;40;100m%[0;36;5;40;100mt[0;33;5;40;100mt[0;36;5;40;100m;;t[0m  \n"
"   [0;36;5;40;100mtt[0;33;5;40;100mtt[0;34;5;40;100mS[0;33;5;40;100m%[0;37;5;43;103m888@[0;1;33;93;47m8[0;33;5;40;100mt[0;36;5;40;100m%%[0;33;5;40;100mt[0;36;5;40;100mttttttt[0;33;5;40;100mtt[0;36;5;40;100m%[0;33;5;40;100mt[0;37;5;43;103m888888888@@XXXXXXX[0;33;5;43;103mt[0;37;43mX[0;33;5;40;100mS[0;34;5;40;100mSS[0;33;5;40;100mt[0;36;5;40;100mtttttttttt[0;33;5;40;100mt[0;34;5;40;100mS[0;33;5;40;100m@@[0;33;5;43;103m:..[0;1;33;93;43mtt[0;33;5;40;100m%[0;36;5;40;100mt[0;33;5;40;100mt[0;36;5;40;100m;;t[0m  \n"
"   [0;36;5;40;100mt;[0;33;5;40;100mtt[0;34;5;40;100mS[0;33;5;40;100mt[0;37;5;43;103m888@[0;1;33;93;47m8[0;33;5;40;100mt[0;36;5;40;100m%%[0;33;5;40;100mt[0;36;5;40;100mttttttttt[0;35;5;40;100mt[0;34;5;40;100m%[0;33;47m8[0;37;5;43;103m@@888888@@@@[0;33;5;43;103m@[0;37;43mX[0;37;5;43;103mXSSS[0;33;5;43;103m.:[0;1;30;90;43m88[0;36;5;40;100m%[0;33;5;40;100mt[0;36;5;40;100mttttttttt[0;33;5;40;100mt[0;34;5;40;100mS[0;33;5;40;100m@@[0;33;5;43;103m...[0;1;33;93;43mtt[0;33;5;40;100m%[0;36;5;40;100mt[0;33;5;40;100mt[0;36;5;40;100m;;t[0m  \n"
"   [0;36;5;40;100mt;[0;33;5;40;100mtt[0;36;5;40;100m%[0;33;5;40;100mt[0;1;33;93;47m8[0;37;5;43;103m8888[0;33;5;40;100mt[0;34;5;40;100mSS[0;33;5;40;100mt[0;36;5;40;100mttttttttt[0;33;5;40;100mt[0;34;5;40;100mS[0;33;5;40;100mt[0;37;5;43;103m8888888[0;33;5;40;100mStt[0;36;5;40;100mtt%%[0;33;5;40;100m%[0;37;43mXX[0;37;5;43;103mXSS[0;33;5;43;103m..[0;33;5;40;100mX[0;34;5;40;100mS[0;33;5;40;100mt[0;36;5;40;100mtttttttt[0;33;5;40;100mt[0;34;5;40;100mS[0;1;30;90;43m88[0;33;5;43;103m...[0;1;33;93;43m;;[0;36;5;40;100mtt[0;33;5;40;100mt[0;36;5;40;100m;;t[0m  \n"
"   [0;36;5;40;100mt;tt[0;33;5;40;100mt[0;36;5;40;100m%[0;33;47m8[0;37;5;43;103m888@[0;37;43m8[0;34;5;40;100mSS[0;33;5;40;100mt[0;36;5;40;100mttttttttt[0;33;5;40;100mt[0;36;5;40;100mt[0;33;5;40;100mt[0;1;33;93;47m88[0;37;5;43;103m@8888[0;33;5;40;100m%[0;34;5;40;100mX[0;36;5;40;100mttt[0;33;5;40;100mttS[0;37;5;43;103m@@XSSSS[0;1;30;90;43m8[0;34;5;40;100mS[0;33;5;40;100mt[0;36;5;40;100mttttttttt[0;33;5;40;100mt[0;1;33;93;43m;;[0;33;5;43;103m...[0;1;30;90;43m88[0;34;5;40;100mS[0;33;5;40;100mt[0;36;5;40;100mttt[0m   \n"
"    [0;36;5;40;100mttt[0;33;5;40;100mt[0;34;5;40;100mS[0;33;5;40;100mt[0;37;5;43;103m88888[0;33;5;40;100mtt[0;34;5;40;100mS[0;33;5;40;100mt[0;36;5;40;100mttttttttt[0;33;5;40;100mt[0;34;5;40;100mS[0;33;5;40;100mtt[0;37;5;43;103m@88888[0;33;47m88[0;37;5;43;103m88@@@XXXXS[0;1;30;90;43m88[0;33;5;40;100mt[0;36;5;40;100mttttttttt[0;33;5;40;100mt[0;36;5;40;100mt[0;33;5;40;100m%[0;1;33;93;43m;;[0;33;5;43;103m..[0;1;33;93;43m;[0;33;5;40;100mSS[0;36;5;40;100m%[0;33;5;40;100mt[0;36;5;40;100m;tt[0m   \n"
"    [0;36;5;40;100mt;;[0;33;5;40;100mtt[0;34;5;40;100mS[0;1;30;90;43m88[0;37;5;43;103m88@[0;1;33;93;47m88[0;33;5;40;100m;[0;36;5;40;100m%[0;33;5;40;100mt[0;36;5;40;100mtttttttt[0;33;5;40;100mt[0;36;5;40;100m%[0;33;5;40;100mtt[0;1;33;93;47m8[0;37;5;43;103m@88888888@@@[0;33;5;43;103mXX[0;37;43m@[0;33;5;40;100mS%[0;36;5;40;100m%%[0;33;5;40;100mt[0;36;5;40;100mt[0;33;5;40;100mttt[0;36;5;40;100mtttttt[0;33;5;40;100mt[0;36;5;40;100mt[0;33;5;40;100mttSSt[0;36;5;40;100mtttt;[0m     \n"
"     [0;36;5;40;100mtttt%[0;33;5;40;100mtt[0;1;33;93;47m8[0;37;5;43;103m88XX[0;33;47m8[0;36;5;40;100m%tttttttttt[0;33;5;40;100mt[0;34;5;40;100mSS[0;37;43m8[0;37;5;43;103m%X@@88[0;33;47m8[0;37;5;43;103m88@@[0;33;5;40;100m%[0;34;5;40;100m@@S[0;36;5;40;100mt[0;33;5;40;100mt[0;36;5;40;100mttt[0;31;5;40;100mS[0;36;5;40;100m%[0;37;46m@@[0;1;30;90;46m888888[0;36;5;40;100m%[0;31;5;40;100mS[0;36;5;40;100mtt[0;33;5;40;100mtt[0;36;5;40;100mtttt;t[0m     \n"
"       [0;36;5;40;100m;t[0;33;5;40;100mt[0;34;5;40;100mSS[0;33;5;40;100mt[0;37;5;43;103m8888@[0;33;47m8[0;33;5;40;100mt[0;36;5;40;100m%%[0;33;5;40;100mt[0;36;5;40;100mtttttt[0;33;5;40;100mt[0;36;5;40;100mtt[0;33;5;40;100mtt;tttt[0;34;5;40;100mS[0;1;30;90;43m88[0;37;5;43;103m88[0;1;30;90;43m8[0;34;5;40;100mSS[0;33;5;40;100mt[0;36;5;40;100mttttt[0;33;5;40;100m%[0;36;5;40;100mtSS%S[0;37;46m@[0;1;36;96;47m88[0;37;46m@[0;36;5;40;100mt[0;33;5;40;100m%[0;36;5;40;100mttttttttt[0m      \n"
"        [0;36;5;40;100m;t[0;33;5;40;100mtt[0;36;5;40;100m%[0;33;5;40;100mt[0;1;33;93;47m8[0;37;5;43;103m@@8@[0;1;33;93;47m8[0;33;5;40;100m;;t[0;36;5;40;100mt[0;33;5;40;100mt[0;36;5;40;100mttttttt[0;33;5;40;100mttttttt[0;36;5;40;100mt[0;33;5;40;100mtttt[0;36;5;40;100mttttttttttt[0;33;5;40;100m%%[0;36;5;40;100mt[0;1;30;90;46m8[0;37;46m@[0;36;5;40;100m%%t[0;33;5;40;100m%[0;36;5;40;100mttttt[0;33;5;40;100mt[0;36;5;40;100mttt[0m       \n"
"         [0;36;5;40;100mttt[0;33;5;40;100mt[0;35;5;40;100mt[0;36;5;40;100mt[0;1;30;90;43m88[0;37;5;43;103m8@8@@[0;1;33;93;47m8[0;33;5;40;100m:t[0;36;5;40;100m%%[0;33;5;40;100mtt[0;36;5;40;100mttttttttttt[0;33;5;40;100mtttt[0;36;5;40;100mttttttttt[0;33;5;40;100mt[0;36;5;40;100mt%%[0;37;46m@@[0;36;5;40;100m%tttttttttttt[0m        \n"
"          [0;33;5;40;100mttt[0;36;5;40;100mt[0;33;5;40;100mt[0;36;5;40;100m%%[0;33;5;40;100mt[0;37;43m8[0;37;5;43;103m8@@8@8[0;33;47m88[0;33;5;40;100m:t[0;36;5;40;100mt%%tt[0;33;5;40;100mttttttttttttt[0;36;5;40;100mttt[0;33;5;40;100mtt[0;36;5;40;100mt[0;31;5;40;100mS[0;36;5;40;100m%[0;37;46m@@@@@@@@[0;36;5;40;100m%[0;31;5;40;100mS[0;36;5;40;100mtt[0;33;5;40;100mt[0;36;5;40;100mtt[0m          \n"
"             [0;36;5;40;100mtt[0;33;5;40;100mttt[0;36;5;40;100m%[0;33;5;40;100mt;;[0;1;33;93;47m8[0;37;5;43;103m@@@@X@[0;1;33;93;47m8[0;33;47m88[0;1;30;90;43m8[0;33;5;40;100mtt;;tttttttt%%S[0;1;30;90;43m88[0;33;5;40;100m%%[0;36;5;40;100mt[0;33;5;40;100mtt[0;36;5;40;100mttttttt[0;33;5;40;100mtt[0;36;5;40;100mt[0;33;5;40;100mtt[0;36;5;40;100mt[0m            \n"
"              [0;36;5;40;100mt;;[0;33;5;40;100mt[0;36;5;40;100mt[0;33;5;40;100mt[0;36;5;40;100m%%[0;34;5;40;100mS[0;33;5;40;100mt[0;1;33;93;47m8[0;37;5;43;103mSSSX@88@XX88888888@@@@XXX[0;33;5;43;103mXX[0;33;5;40;100mt[0;36;5;40;100mttttttttt[0;33;5;40;100mt[0;36;5;40;100mt;tt[0m             \n"
"                 [0;36;5;40;100mttt[0;33;5;40;100mttt[0;36;5;40;100m%[0;33;5;40;100mttt:[0;33;47m8[0;1;33;93;47m8[0;37;5;43;103m88@@888888888@@@@@XX[0;1;30;90;43m88[0;36;5;40;100mtttttt[0;33;5;40;100mtt[0;36;5;40;100mtttt[0m                \n"
"                    [0;36;5;40;100mtttt[0;33;5;40;100mttt[0;36;5;40;100mt%%[0;33;5;40;100mtt;t;;;[0;1;30;90;43m8[0;37;43m8888[0;1;30;90;43m888[0;33;5;40;100m%%%%t[0;36;5;40;100mttttttttt;[0m                    \n"
"                        [0;36;5;40;100mtttt[0;33;5;40;100mtttttt[0;36;5;40;100mttttt%%%ttttt[0;33;5;40;100mttt[0;36;5;40;100mttttttt[0m                       \n"
"                                [0;36;5;40;100mttttttttttttttttt[0m                               \n";
