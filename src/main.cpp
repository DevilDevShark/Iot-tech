#include <HardwareSerial.h>
#include <bits/stdc++.h>
#include <set>
using namespace std;

int ledSensor = 2;
int DEFAULT_VALUE;
int lightSensor = 0;
int i = 0;

const String strState[] = {
    "DEFAULT",
    "START",
    "BIT_1",
    "BIT_2",
    "BIT_3",
    "DATA",
    "DATAR",
    "FIN",
};

String stateToString(int state)
{
  return strState[state];
}

enum State
{
  DEFAULT,
  START,
  BIT_1,
  BIT_2,
  BIT_3,
  DATA,
  DATA_R,
  FIN,
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
set<int> arr;
std::set<int>::iterator it;
int median;

void RunFsm()
{
  fsm.checkState(DEFAULT, START, timer1._elapsed() > 2000);
  fsm.checkState(START, BIT_1, median != 0, &timer1);
  fsm.checkState(BIT_1, BIT_2, sensorState == 1, &timer1);
  fsm.checkState(BIT_2, BIT_3, sensorState == 1, &timer1);
  fsm.checkState(BIT_3, DATA, sensorState == 0, &timer1);
  fsm.checkState(DATA, DATA_R, true, &timer1);
  fsm.checkState(DATA_R, FIN, DATA_R == !DATA ? true : false, &timer1);
}

int ReadInput(int median)
{
  if (analogRead(ledSensor) >= median*1.5 || analogRead(ledSensor) >= 4095)
  {
    return 1;
  }
  else
  {
    return 0;
  }
}

String typeLaser(int data)
{
  if (data = 1) {
    return "terrestre";
  } else {
    return "aerien";
  }
}

void setup()
{
  Serial.begin(9600);
  pinMode(ledSensor, INPUT);
  Serial.println("Starting");
}

void loop()
{
  RunFsm();

  switch (fsm.getCurrentState())
  {
  case DEFAULT:
    while (arr.size() < 17)
    {
      arr.emplace(analogRead(ledSensor));

      if (arr.size() > 16)
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
    it = arr.begin();
    std::advance(it, arr.size() / 2);
    cout << "mediane" << *it;
    median = *it;
    
    break;
  case BIT_1:
    sensorState = ReadInput(median);
    break;
  case BIT_2:
    sensorState = ReadInput(median);
    break;
  case BIT_3:
    sensorState = ReadInput(median);
    break;
  case DATA:
    sensorState = ReadInput(median);
    Serial.println(sensorState);
    break;
  case DATA_R:
    sensorState = ReadInput(median);
    Serial.println("message reçu");
    break;
  };
}
