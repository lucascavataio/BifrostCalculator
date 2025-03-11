#include <Arduino.h>
#include <math.h>

// Define our own "fabs" in case we are using an Arduino UNO 
// or any other controller were the standard math library isn’t automatically linked.
#ifdef __AVR__
extern "C" double fabs(double x) {
  return (x < 0) ? -x : x;
}
#endif

#include "tinyexpr.h"  // Ensure you've installed the Tinyexpr library.

// Global variable for the target buffer size
int targetBufferSize = 200;

void setup() {
  Serial.begin(9600);  // Initialize serial communication at 9600 baud
  while (!Serial) {
    ;  // Wait for serial port to connect (if needed)
  }
}

void loop() {
  if (Serial.available()) {
    // Read a line from serial input
    String input = Serial.readStringUntil('\n');
    input.trim();  // Remove any extra whitespace

    // Otherwise, treat the input as a mathematical expression.
    // Dynamically allocate a buffer using the target buffer size.
    char* expr = new char[targetBufferSize];
    input.toCharArray(expr, targetBufferSize);

    // Define the variables needed for TinyExpr.
    const double pi_value = PI;
    te_variable vars[] = { {"pi", &pi_value} };

    // Compile the expression with the variables.
    int err;
    te_expr* n = te_compile(expr, vars, 1, &err);

    if (!n) {
      //The message contains "nan" since is the identifier for detecting the error message.
      Serial.print("nanSyntax error at position: ");
      Serial.println(err);
    } else {
      // Evaluate the compiled expression.
      double result = te_eval(n);
      te_free(n);

      // Check for infinity (e.g., division by zero)
      if (isinf(result)) {
        Serial.println("inf");
      } else {
        Serial.println(result, 6);  // Print result with 6 decimal places
      }
    }

    // Free the dynamically allocated memory.
    delete[] expr;
  }
}
