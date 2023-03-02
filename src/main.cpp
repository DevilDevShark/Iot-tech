#include <HardwareSerial.h>
#include <bits/stdc++.h>
#include <set>
using namespace std;

int ledSensor = 2;

const String strState[] = {
    "DEFAULT",
    "START",
    "BIT_1",
    "BIT_2",
    "BIT_3",
    "DATA",
    "DATAR",
    "END",
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
  END,
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
set<int> valuesArray;
std::set<int>::iterator arrayIterator;
int median = 0;
bool data;

void RunFsm()
{
  fsm.checkState(DEFAULT, START, timer1._elapsed() > 2000);
  fsm.checkState(START, BIT_1, median != 0, &timer1);
  fsm.checkState(BIT_1, BIT_2, sensorState == 1, &timer1);
  fsm.checkState(BIT_2, BIT_3, sensorState == 1, &timer1);
  fsm.checkState(BIT_3, DATA, sensorState == 0, &timer1);
  fsm.checkState(DATA, DATA_R, true, &timer1);
  fsm.checkState(DATA_R, END, data != sensorState ? true : false, &timer1);
}

int readInput(int median)
{
  // Compare la lumière ambiante avec la médiane. Une marge de 50%
  if (analogRead(ledSensor) >= median * 1.5 || analogRead(ledSensor) >= 4095)
  {
    return 1;
  }
  else
  {
    return 0;
  }
}

void typeLaser(int sensorLed)
{
  if (sensorLed == 1)
  {
    data = 1;
  }
  else
  {
    data = 0;
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
    // Récolté tant que la taille du tableau est inférieur à 17
    while (valuesArray.size() < 17)
    {
      // Enregistre les données dans un tableau ordonné
      valuesArray.emplace(analogRead(ledSensor));

      // Chelou comme condition 🤔
      if (valuesArray.size() > 16)
      {
        for (int i : valuesArray)
        {
          cout << i << ' ';
        }

        cout << "SIZE " << valuesArray.size() << " ";
      }
    }

    break;
  case START:
    // Connaitre la médiane des données récoltées précedement
    arrayIterator = valuesArray.begin();
    std::advance(arrayIterator, valuesArray.size() / 2);
    cout << "mediane" << *arrayIterator;
    median = *arrayIterator;

    break;
  case BIT_1:
    // Détecte si y'a un laser
    sensorState = readInput(median);
    break;
  case BIT_2:
    sensorState = readInput(median);
    break;
  case BIT_3:
    sensorState = readInput(median);
    break;
  case DATA:
    sensorState = readInput(median);
    typeLaser(sensorState);
    break;
  case DATA_R:
    sensorState = readInput(median);
    break;
  case END:
    Serial.println("Message reçu :  laser " + data ? "terrestre" : "aérien");
    break;
  };
}
