package edu.afit.scpsolver;

import edu.afit.scpsolver.SCPSolver;
import edu.afit.scpsolver.ast.SCP;
import edu.afit.scpsolver.parse.SCPParser;
import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.io.InputStreamReader;

public class Main {
	private static BufferedReader inputBuffer = null;
	private static BufferedWriter outputBuffer = null;
	private static BufferedWriter dotBuffer = null;

	public static void main(String[] args) {
		BufferedReader in = new BufferedReader(new InputStreamReader(System.in));

		try {
			System.out.println("***************************************************");
			System.out.println("*         Welcome to the SCP Solver 2006          *");
			System.out.println("*                                                 *");
			System.out.println("*                 by Walter Hayden                *");
			System.out.println("***************************************************");
			System.out.println("\nPlease enter file name of the input file below: \n");
			System.out.print("Filename: ");
			String e = in.readLine();
			inputBuffer = new BufferedReader(new FileReader(e));
			System.out.print("\nWould you like to output to a file? (Y/N):  ");
			String outputAnswer = in.readLine();
			String dotAnswer;
			if (outputAnswer.equalsIgnoreCase("y")) {
				System.out.println("\nPlease enter file name for the output file below: \n");
				System.out.print("Filename: ");
				dotAnswer = in.readLine();
				outputBuffer = new BufferedWriter(new FileWriter(dotAnswer));
			}

			System.out.print(
					"\nWould you like the create a dot (graph) file? (Y/N): \n  Note: NOT recommended for large input! ");
			dotAnswer = in.readLine();
			// INSERTING TIME MEASUREMENT
			long startTime = System.nanoTime() / 1000;
			long duration = System.nanoTime() / 1000;

			if (dotAnswer.equalsIgnoreCase("y")) {
				System.out.println("\nPlease enter the dot file name below: \n");
				System.out.print("Filename: ");
				String scpp = in.readLine();
				dotBuffer = new BufferedWriter(new FileWriter(scpp));
			}

			if (inputBuffer != null) {
				SCPParser scpp1 = new SCPParser(inputBuffer);

				try {
					SCP e1 = scpp1.SCP();
					SCPSolver solver = new SCPSolver(e1, outputBuffer, dotBuffer);
					if (outputBuffer != null) {
						outputBuffer.write("Input file contents:\n");
						outputBuffer.write("\n" + e1.toString() + "\n");
					}

					if (solver.solveSCP()) {
						// MEASURE THE TOTAL DURATION (MINUS FILE WRITING)
						duration = System.nanoTime()/1000 - startTime;
						if (outputBuffer != null) {
							outputBuffer.write("\nThe Solution Set: " + solver.getSolutionIDSet().toString() + "\n");
							outputBuffer.write("Total cost: " + solver.getSolutionCost() + "\n");
							// MEASURE THE TOTAL DURATION (INCLUDING MOST OF THE FILE WRITING)
							//duration = System.nanoTime()/1000 - startTime;
							outputBuffer.write("Duration: " + duration + " µs\n");
						}

						System.out.println("\nThe Solution Set: " + solver.getSolutionIDSet().toString());
						System.out.println("Total cost: " + solver.getSolutionCost());
						System.out.println("Duration: " + duration + " µs");
					} else {
						System.out.println("The SCP found in file could not be solved.");
					}
				} catch (Throwable arg15) {
					arg15.getMessage();
					arg15.printStackTrace();
				}
			} else {
				System.out.println("The input stream is null!");
				System.exit(0);
			}
		} catch (IOException arg16) {
			arg16.getMessage();
			arg16.printStackTrace();
		} finally {
			try {
				if (inputBuffer != null) {
					inputBuffer.close();
				}

				if (outputBuffer != null) {
					outputBuffer.close();
				}

				if (dotBuffer != null) {
					dotBuffer.close();
				}
			} catch (IOException arg14) {
				arg14.getMessage();
				arg14.printStackTrace();
			}

		}

	}
}