#include "Context.h"
#include "Coroutine.h"
#include <iostream>

using namespace std;

class MyCoroutine : public Coroutine
{
protected:
    virtual void* Main(void* param)
    {
        cout << "Coroutine: 1" << endl;
        Yield();
        cout << "Coroutine: 2" << endl;
        Yield();
        cout << "Coroutine: 3" << endl;
        Yield();
        cout << "Coroutine: 4" << endl;
        return nullptr;
    }
};

int main()
{
    MyCoroutine coroutine;

    cout << "Main: 1" << endl;
    coroutine.StartCoroutine(nullptr, 1024 * 1024);
    cout << "Main: 2" << endl;
    coroutine.Resume();
    cout << "Main: 3" << endl;
    coroutine.Resume();
    cout << "Main: 4" << endl;
    coroutine.Resume();

    return 0;
}