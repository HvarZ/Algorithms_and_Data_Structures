import java.util.Scanner;
import java.io.OutputStream;
import java.io.OutputStreamWriter;
import java.io.BufferedWriter;

public class MainStack {
    public static class Stack {
        private String[] data_;
        private int capacity_;
        private int size_;

        public Stack(int capacity_) {
            this.data_ = new String[capacity_];
            this.capacity_ = capacity_;
            this.size_ = 0;
        }

        public void push(String value) throws Exception {
            if (size_ >= capacity_) {
                throw new Exception("overflow");
            } else {
                data_[size_++] = value;
            }
        }

        public String pop() throws Exception {
            if (size_ == 0) {
                throw new Exception("underflow");
            } else {
                return data_[--size_];
            }
        }

        public void print(OutputStream out) throws Exception {
            if (size_ == 0) {
                throw new Exception("empty");
            } else {
                BufferedWriter writer = new BufferedWriter(new OutputStreamWriter(out));
                for (int i = 0; i < size_ - 1; ++i) {
                    writer.write(data_[i] + " ");
                }
                writer.write(data_[size_ - 1] + "\n");
                writer.flush();
            }
        }
    }

    public static void main(String[] args) {
        String regexSet = "^set_size\\s[0-9][0-9]*$";
        String regexCommand = "^push\\s\\S*$|^pop$|^print$|";
        String buffer;
        Stack stack = null;
        Scanner input = new Scanner(System.in);
        while (input.hasNext()) {
            buffer = input.nextLine();
            if (buffer.length() == 0) {
                continue;
            } else if (buffer.matches(regexSet)) {
                String[] splitCommand = buffer.split(" ");
                int sizeStack = Integer.parseInt(splitCommand[1]);
                stack = new Stack(sizeStack);
                break;
            } else {
                System.out.println("error");
            }
        }
        while (input.hasNext()) {
            buffer = input.nextLine();
            if (buffer.length() == 0) {
                continue;
            } else if (buffer.matches(regexCommand)) {
                String[] splitCommand = buffer.split(" ");
                if (splitCommand[0].equals("push")) {
                    try {
                        stack.push(splitCommand[1]);
                    } catch (Exception ex) {
                        System.out.println("overflow");
                    }
                } else if (splitCommand[0].equals("pop")) {
                    try {
                        System.out.println(stack.pop());
                    } catch (Exception ex) {
                        System.out.println("underflow");
                    }
                } else if (splitCommand[0].equals("print")) {
                    try {
                        stack.print(System.out);
                    } catch (Exception ex) {
                        System.out.println("empty");
                    }
                } else {
                    System.out.println("error");
                }
            } else {
                System.out.println("error");
            }
        }
    }
}
