#include <HardwareSerial.h>
#include <bits/stdc++.h>
#include <vector>
#include <set>
using namespace std;

int ledSensor = 2;
int DEFAULT_VALUE;
int lightSensor = 0;
int i = 0;

const String strState[] = {
    "DEFAULT",
    "START",
    "BITO",
    "BITZ",
    "DATA",
    "DATAR",
};

String stateToString(int state)
{
  return strState[state];
}

enum State
{
  DEFAULT,
  START,
  BITO,
  BITZ,
  DATA,
  DATAR,
};

class Timer
{
public:
  Timer();
  unsigned long elapsed = 0;
  unsigned long startTime = 0;

  void start();

  unsigned long _elapsed();
};

Timer::Timer()
{
}

void Timer::start()
{
  startTime = millis();
}

unsigned long Timer::_elapsed()
{
  return millis() - startTime;
}

class FSM
{
  State mCurrentState = State::DEFAULT;

public:
  FSM();

  void checkState(State source, State target, bool condition = true, Timer *timer = nullptr);

  State getCurrentState();
};

FSM::FSM() : mCurrentState(DEFAULT)
{
}

void FSM::checkState(State source, State target, bool condition, Timer *timer)
{

  if (source == mCurrentState && condition)
  {
    Serial.println(stateToString(source) + " -> " + stateToString(target) + ":" + condition);
    if (timer != nullptr)
    {
      timer->start();
    }
    mCurrentState = target;
  }
}

State FSM::getCurrentState()
{
  return mCurrentState;
}

FSM fsm;
Timer timer1;
int sensorState = 0;
int incrnumber = 0;
bool isIncr = true;

void RunFsm()
{
  fsm.checkState(DEFAULT, START, timer1._elapsed() > 2000);
  fsm.checkState(DEFAULT, START, true, &timer1);
  // fsm.checkState(START, BITO, sensorState == 1, &timer1);
  // fsm.checkState(BITO, BITZ, sensorState == 1, &timer1);
  // fsm.checkState(BITZ, DATA, &timer1);
  // fsm.checkState(DATA, DATAR, DATAR == !DATA ? true : false);
}

int ReadInput()
{
  if (digitalRead(ledSensor) > DEFAULT_VALUE)
  {
    return 1;
  }
  else
  {
    return 0;
  }
}

set<int> arr;
int n;
auto it = arr.begin();

void setup()
{
  Serial.begin(9600);
  pinMode(ledSensor, INPUT);
  Serial.println("Starting");
}

void loop()
{
  sensorState = ReadInput();
  RunFsm();

  switch (fsm.getCurrentState())
  {
  case DEFAULT:
    while (arr.size() < 17)
    {
      arr.emplace(analogRead(ledSensor));

      if (arr.size() == 16)
      {
        for (int i : arr)
        {
          cout << i << ' ';
        }

        cout << "SIZE " << arr.size() << " ";
      }
    }

    break;
  case START:
    if (arr.size() % 2 == 1)
    {
      n = (arr.size() / 2) - 1;
    }
    else if (arr.size() % 2 == 0)
    {
      n = arr.size() / 2;
    }

    for (int i = 0; i < n; i++)
    {
      it++;
    }

    cout << "Élément à index " << *it;
    break;
  case BITO:
    break;
  case BITZ:
    break;
  case DATA:
    break;
  case DATAR:
    Serial.println("message reçu");
    break;
  };
}
