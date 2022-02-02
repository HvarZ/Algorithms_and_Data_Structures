import java.util.Stack;
import java.util.Scanner;

enum Step {
    INC,
    DBL,
    DEC
}

public class CrazyRichManGame {
    public static int getUnitsInNumber(int number) {
        int result = 0;
        while (number > 0) {
            result += number & 1;
            number >>= 1;
        }
        return result;
    }

    public static Stack<Step> getSteps(int number) {
       Stack<Step> stack = new Stack<>();
       while (number > 1 && number != 3) {
            if (number % 2 == 1) {
                if (getUnitsInNumber(number - 1) < getUnitsInNumber(number + 1)) {
                    --number;
                    stack.push(Step.INC);
                }
                else {
                    ++number;
                    stack.push(Step.DEC);
                }
            }
            else {
                number >>= 1;
                stack.push(Step.DBL);
            }
       }

       if (number == 3) {
           stack.push(Step.INC);
           stack.push(Step.DBL);
       }

       stack.push(Step.INC);
       return stack;
    }

    public static void main(String[] args) {
        int number = 0;
        Scanner input = new Scanner(System.in);
        number = input.nextInt();

        Stack<Step> steps = getSteps(number);
        while (!steps.empty()) {
            Step currentStep = steps.peek();
            steps.pop();
            if (currentStep == Step.INC) {
                System.out.println("inc");
            } else if (currentStep == Step.DBL) {
                System.out.println("dbl");
            } else if (currentStep == Step.DEC) {
                System.out.println("dec");
            }
        }
    }
}
