# SimpleNetRoutine-auto
This is a soroutine library with an auto-scheduler.The monitoring aspect is not finished yet...

```
    typedef struct
    {
        int a;
    } Test;

    void test1(void *args)
    {
        Test *t = (Test *)args;
        //std::cout << t->a << std::endl;
        // pthread_kill(pthread_self(), SIGUSR2);
        std::cout << "send sig to id:" << pthread_self()<< " A" << std::endl;
        int i = 0;
        while (1)
        {
            /* code */
            // if (!(i++ % 40000))
            std::this_thread::sleep_for(std::chrono::seconds(1));
            std::cout << "i am A :" << i++ << std::endl;
        }
        delete (Test *)t;
    }

    void test2(void *args)
    {
        Test *t = (Test *)args;
        // std::cout << t->a << std::endl;
        // pthread_kill(pthread_self(), SIGUSR2);
        std::cout << "send sig to id:" << pthread_self() << " B" << std::endl;
        int i = 0;
        while (1)
        {
            /* code */
            // if (!(i++ % 40000))
            std::this_thread::sleep_for(std::chrono::seconds(1));
            std::cout << "i am B :" << i++ << std::endl;
        }
        delete (Test *)t;
    }

    void test3(void *args)
    {
        Test *t = (Test *)args;
        // std::cout << t->a << std::endl;
        //  pthread_kill(pthread_self(), SIGUSR2);
        std::cout << "send sig to id:" << pthread_self() << " C" << std::endl;
        int i = 0;
        while (1)
        {
            /* code */
            // if (!(i++ % 40000))
            std::this_thread::sleep_for(std::chrono::seconds(1));
            std::cout << "i am C :" << i++ << std::endl;
        }
        delete (Test *)t;
    }

    int main()
    {
        Test *t1 = new Test();
        t1->a = 1;
        Test *t2 = new Test();
        t2->a = 2;
        Test *t3 = new Test();
        t2->a = 3;
        simple_start(test1, t1);
        simple_start(test2, t2);
        simple_start(test3, t3);
        std::this_thread::sleep_for(std::chrono::seconds(30));
        for (;;)
        {
        }
        return 0;
    }
```




![image](https://user-images.githubusercontent.com/122962161/236869743-e4382352-2a06-4f11-8ef2-87c84ef95482.png)
