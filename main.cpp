#include "mbed.h"
#include "TB6612.h"
#include "LineTrace.h"
#include "IrRemoteControl.h"

#define MOVE_LENGTH 2
#define REPLACE_MOVE \
    {                \
        1, 3         \
    }

#define COLOR_LENGTH 2
#define REPLACE_COLOR            \
    {                            \
        {0, 3, 100}, { 5, 2, 0 } \
    }

#define INITIAL_COLOR \
    {                 \
        195, 3, 2     \
    }

// main() runs in its own thread in the OS
int main()
{
    // DigitalIn pin(PF_0);
    int move[] = REPLACE_MOVE;
    int color[][3] = REPLACE_COLOR;

    IrRemoteControl controller(PF_0);

    TB6612 tb6612(PA_5, PA_4, PA_8, PA_7, PB_0, PB_1, PA_6);
    LineTrace line_trace(PA_0, PA_1, PA_3);

    DigitalOut led1(PB_6);
    DigitalOut led2(PB_7);

    for (int i = 0; i < 10; i++)
    {
        led1 = !led1;
        led2 = !led2;
        ThisThread::sleep_for(100ms);
    }

    // while(1){
    //     int value = controller.get_reader_code();
    //     led1 = (value & 1);
    //     led2 = (value & 2);
    //     ThisThread::sleep_for(10ms);
    // }

    tb6612.standby(1);

    for (int i = 0; i < 10; i++)
    {
        led1 = !led1;
        led2 = !led2;
        ThisThread::sleep_for(100ms);
    }

    while (true)
    {
        Motor motor[2];

        int result = line_trace.read();

        // if (result == 0)
        // {
        //     // // TurnRight
        //     // motor[0].set_state(State::CCW);
        //     // motor[0].set_duty_cycle(0.50f);
        //     // motor[1].set_state(State::CCW);
        //     // motor[1].set_duty_cycle(0.20f);

        //     // tb6612.set(motor[0], 0);
        //     // tb6612.set(motor[1], 1);

        //     // ThisThread::sleep_for(700ms);

        //     // TurnLeft
        //     motor[0].set_state(State::Brake);
        //     motor[0].set_duty_cycle(0.00f);
        //     motor[1].set_state(State::CW);
        //     motor[1].set_duty_cycle(0.50f);

        //     tb6612.set(motor[0], 0);
        //     tb6612.set(motor[1], 1);

        //     ThisThread::sleep_for(700ms);
        // }

        if (controller.get_reader_code() == 0)
        {
            motor[0].set_state(State::Brake);
            motor[0].set_duty_cycle(0.00f);
            motor[1].set_state(State::Brake);
            motor[1].set_duty_cycle(0.00f);
        }

        motor[0].set_state(line_trace.get_left_state());
        motor[0].set_duty_cycle(line_trace.get_left_duty_cycle());
        motor[1].set_state(line_trace.get_right_state());
        motor[1].set_duty_cycle(line_trace.get_right_duty_cycle());

        tb6612.set(motor[0], 0);
        tb6612.set(motor[1], 1);

        led1 = !led1;
        led2 = !led2;
        ThisThread::sleep_for(10ms);
    }
}
