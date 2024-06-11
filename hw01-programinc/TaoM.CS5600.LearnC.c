/*
* TaoM.CS5600.LearnC.c / Program in C
*
* Minjia Tao / CS5600 / Northeastern University
* Spring 2024 / Jan 26, 2024
*
*/

#include <stdio.h>
#include <math.h>
#include "make.h"

// ... (Your function definitions here)

int main(void) {
    // Define a small value for comparison of floating-point numbers
    const float EPSILON = 0.001;

    // Define test cases and expected results
    struct {
        float input;
        float expected;
        float (*conversionFunc)(float);
        char *testName;
    } tests[] = {
        {30.2,  mpg2kml(30.2),  mpg2kml, "mpg to km/l test 1"},
        {-1.5,  mpg2kml(-1.5),  mpg2kml, "mpg to km/l test 2"},
        {20,    mpg2kml(20),    mpg2kml, "mpg to km/l test 3"},
        {21.5,  lph2mpg(21.5),  lph2mpg, "lph to mpg test 1"},
        {-99.9, lph2mpg(-99.9), lph2mpg, "lph to mpg test 2"},
        {100,   lph2mpg(100),   lph2mpg, "lph to mpg test 3"},
        {60.1,  mpg2lphm(60.1), mpg2lphm, "mpg to l/100km test 1"},
        {-27.9, mpg2lphm(-27.9),mpg2lphm, "mpg to l/100km test 2"},
        {300,   mpg2lphm(300),  mpg2lphm, "mpg to l/100km test 3"},
    };

    // Loop through each test
    for (int i = 0; i < sizeof(tests) / sizeof(tests[0]); i++) {
        // Call the conversion function
        float actual = tests[i].conversionFunc(tests[i].input);

        // Check if the actual result is close enough to the expected result
        if (fabs(actual - tests[i].expected) < EPSILON) {
            printf("%s: The expected value is %f, the actual value is %f, test passed.\n",
                   tests[i].testName, tests[i].expected, actual);
        } else {
            printf("%s: The expected value is %f, the actual value is %f, test failed.\n",
                   tests[i].testName, tests[i].expected, actual);
        }
    }

    return 0;
}



// // testing
// #include <stdio.h>
// #include "make.h"

// /* main.c */
// int main(void) {
//     // call function in another file
//     float testmpg1 = 30.2;
//     float res1 = mpg2kml(testmpg1);
//     printf("%f\n", res1);

//     float testmpg2 = -1.5;
//     float res2 =  mpg2kml(testmpg2);
//     printf("%f\n", res2);

//     float testmpg3 = 20;
//     float res3 = mpg2kml(testmpg3);
//     printf("%f\n", res3);

//     float testlph1 = 21.5;
//     float res4 = lph2mpg(testlph1);
//     printf("%f\n", res4);

//     float testlph2 = -99.9;
//     float res5 = lph2mpg(testlph2);
//     printf("%f\n", res5);

//     float testlph3 = 100;
//     float res6 = lph2mpg(testlph3);
//     printf("%f\n", res6);

//     float testmpg4 = 60.1;
//     float res7 = mpg2lphm(testmpg4);
//     printf("%f\n", res7);

//     float testmpg5 = -27.9;
//     float res8 = mpg2lphm(testmpg5);
//     printf("%f\n", res8);

//     float testmpg6 = 300;
//     float res9 = mpg2lphm(testmpg6);
//     printf("%f\n", res9);

//     return 0;
// }
