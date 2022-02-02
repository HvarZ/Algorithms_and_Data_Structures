import java.util.BitSet;
import java.util.ArrayList;
import java.util.Scanner;

public class Backpack {
    public static class Gem {
        public int weight;
        public int cost;

        public Gem(int weight_, int cost_) {
            this.weight = weight_;
            this.cost = cost_;
        }
    }

    public static class ResultBackpack {
        public int weight;
        public int cost;
        public BitSet trace;

        public ResultBackpack(int weight_, int cost_, int size_) {
            this.weight = weight_;
            this.cost = cost_;
            this.trace = new BitSet(size_);
        }
    }

    public static class PairWeightTrace {
        public int weight;
        public BitSet trace;

        public PairWeightTrace(int weight_, int size_) {
            this.weight = weight_;
            this.trace = new BitSet(size_);
        }
    }

    public static ResultBackpack getBackpack(ArrayList<Gem> gems, int maxWeight) {
        ResultBackpack backpack = new ResultBackpack(0, 0, gems.size() + 1);
        int sumWeight = 0;
        int sumCost = 0;
        for (Gem gem : gems) {
            sumWeight += gem.weight;
            sumCost += gem.cost;
        }

        if (maxWeight > sumWeight) {
            backpack.trace.set(1, gems.size() + 1);
            backpack.weight = sumWeight;
            backpack.cost = sumCost;
            return backpack;
        }

        ArrayList<PairWeightTrace> matrix = new ArrayList<>();
        matrix.ensureCapacity(sumCost);
        for (int i = 0; i < sumCost; ++i) {
            matrix.add(new PairWeightTrace(maxWeight + 1, gems.size()));
            matrix.get(i).trace.clear();
        }

        matrix.get(0).weight = 0;
        for (int i = 0; i < gems.size(); ++i) {
            for (int j = sumCost - 1; j >= gems.get(i).cost; --j) {
                if (gems.get(i).weight + matrix.get(j - gems.get(i).cost).weight < matrix.get(j).weight) {
                    matrix.get(j).weight = matrix.get(j - gems.get(i).cost).weight;
                    for (int k = 0; k < gems.size(); ++k) {
                        matrix.get(j).trace.set(k, matrix.get(j - gems.get(i).cost).trace.get(k));
                    }
                    matrix.get(j).weight += gems.get(i).weight;
                    matrix.get(j).trace.set(i + 1);
                }
            }
        }

        for (int i = sumCost - 1; i > 0; --i) {
            if (matrix.get(i).weight <= maxWeight) {
                backpack.weight = matrix.get(i).weight;
                backpack.cost = i;
                backpack.trace = matrix.get(i).trace;
                break;
            }
        }

        return backpack;
    }
    public static void main(String[] args) {
        String buffer;
        String regexWeight = "^[0-9][0-9]*$";
        String gemRegex = "^[0-9][0-9]*\\s[0-9][0-9]*$";
        Scanner input = new Scanner(System.in);
        int maxWeight = 0;
        while (input.hasNext()) {
            buffer = input.nextLine();
            if (buffer.isEmpty()) {
                continue;
            } else if (buffer.matches(regexWeight)) {
                maxWeight = Integer.parseInt(buffer);
                break;
            } else {
                System.out.println("error");
            }
        }

        ArrayList<Gem> gems = new ArrayList<>();

        while (input.hasNext()) {
            buffer = input.nextLine();
            if (buffer.isEmpty()) {
                continue;
            } else if (buffer.matches(gemRegex)) {
                String[] weightCost = buffer.split(" ");
                gems.add(new Gem(Integer.parseInt(weightCost[0]), Integer.parseInt(weightCost[1])));
            } else {
                System.out.println("error");
            }
        }


        if (!gems.isEmpty()) {
            ResultBackpack result = getBackpack(gems, maxWeight);
            System.out.println(result.weight + " " + result.cost);

            for (int i = 0; i < result.trace.length(); ++i) {
                if (result.trace.get(i)) {
                    System.out.println(i);
                }
            }
        }
    }
}
