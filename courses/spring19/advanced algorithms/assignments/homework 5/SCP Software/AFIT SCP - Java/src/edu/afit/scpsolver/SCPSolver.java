package edu.afit.scpsolver;

import edu.afit.scpsolver.ElementsPerBlockFunction;
import edu.afit.scpsolver.IDFunction;
import edu.afit.scpsolver.P;
import edu.afit.scpsolver.StackPair;
import edu.afit.scpsolver.Tableau;
import edu.afit.scpsolver.ast.Family;
import edu.afit.scpsolver.ast.SCP;
import java.io.BufferedWriter;
import java.io.IOException;
import java.util.Iterator;
import java.util.Set;
import java.util.Stack;
import java.util.TreeSet;

public class SCPSolver {
	Set<Family> B;
	Set<Family> B_hat;
	int z;
	int z_hat;
	Set<Integer> E;
	Tableau t;
	boolean output;
	BufferedWriter outputBuffer;
	boolean dot;
	BufferedWriter dotBuffer;

	public SCPSolver(SCP scp) {
		this.B = new TreeSet(new IDFunction());
		this.B_hat = new TreeSet(new IDFunction());
		this.z = 0;
		this.z_hat = Integer.MAX_VALUE;
		this.E = new TreeSet();
		this.output = false;
		this.outputBuffer = null;
		this.dot = false;
		this.dotBuffer = null;
		this.t = new Tableau(scp);
	}

	public SCPSolver(SCP scp, BufferedWriter outputBuffer, BufferedWriter dotBuffer) {
		this(scp);
		if (outputBuffer != null) {
			this.output = true;
			this.outputBuffer = outputBuffer;
		}

		if (dotBuffer != null) {
			this.dot = true;
			this.dotBuffer = dotBuffer;
		}

	}

	public boolean solveSCP() throws IOException {
		if (!this.t.getRNotCovered().isEmpty()) {
			System.out.println("The following could not be covered: " + this.t.getRNotCovered().toString());
			return false;
		} else {
			// heuristic
			// this.t.sortBlocks(new ElementsPerBlockFunction());
			if (this.output) {
				this.outputBuffer.write("\nOriginal Matrix\n\n");
				this.outputBuffer.write(this.t.printOriginalSCP() + "\n");
				this.outputBuffer.write("Reduced Matrix\n");
			}

			boolean keepSearching = true;
			if (this.output) {
				if (!this.t.getS().isEmpty()) {
					this.outputBuffer.write("\n" + this.t.toString() + "\n");
				} else {
					this.outputBuffer.write("\nThe tableau is empty.\n\n");
				}
			}

			if (this.dot) {
				this.dotBuffer.write("digraph g\n{\n\tnode [fontsize = \"16\" shape = \"record\"];\n\n");
				this.dotBuffer.write(
					"\tranksep=\"1.0 equally\";\n\tnodesep=\"1.0 equally\";\n\tcenter=true; ratio=auto;\n\n");
				this.dotBuffer.write("\tLegend [label = \"{ F | {cost|total cost} |\\{E set\\}}\"");
				this.dotBuffer.write(" style=bold,color=dark_green];\n\n\tStart[shape=\"ellipse\"];\n\n");
			}

			this.B.addAll(this.t.getMustBeInBSet());
			this.buildE();
			if (this.E.containsAll(this.t.getR())) {
				this.B_hat.addAll(this.t.getMustBeInBSet());
				this.z_hat = 0;

				Family pChanged;
				for (Iterator p = this.B_hat.iterator(); p.hasNext(); this.z_hat += pChanged.getCost()) {
					pChanged = (Family) p.next();
				}

				keepSearching = false;
			}

			Stack arg10 = new Stack();
			this.t.getS().resetMarker();
			P arg11 = null;
			int nodesConsidered = 0;
			int nodesAdded = 0;
			int betterSolutions = 0;
			boolean keepSearchingBlock = true;
			boolean findP = true;

			while (true) {
				label99 : while (keepSearching) {
					while (findP && this.t.getS().hasNext()) {
						arg11 = this.t.getS().getNext();
						arg11.resetMarker();
						if (!this.E.contains(arg11.getPrimaryElement())) {
							break;
						}
					}

					keepSearchingBlock = true;

					do {
						if (!arg11.hasNext()) {
							continue label99;
						}

						Family f = arg11.getNext();
						++nodesConsidered;
						if (this.z + f.getCost() < this.z_hat) {
							String sp = this.getSuccNode(arg10, false) + f.getId();
							++nodesAdded;
							this.B.add(f);
							this.E.addAll(f.getRSet().getR());
							this.z += f.getCost();
							if (this.dot) {
								this.dotBuffer.write("\t" + sp + " [label = \"{" + f.getId() + "| {" + f.getCost() + "|"
										+ this.z + "} |\\{" + this.getEStr() + "\\}}\"");
							}

							if (!this.E.containsAll(this.t.getR())) {
								if (this.dot) {
									this.dotBuffer.write("];\n");
									this.dotBuffer.write("\t" + this.getSuccNode(arg10, true) + " -> " + sp + ";\n\n");
								}

								arg10.push(new StackPair(arg11, f));
								findP = true;
								continue label99;
							}

							++betterSolutions;
							this.B_hat.clear();
							this.B_hat.addAll(this.B);
							this.z_hat = this.z;
							if (this.dot) {
								this.dotBuffer.write("style=bold,color=blue];\n");
								this.dotBuffer.write("\t" + this.getSuccNode(arg10, true) + " -> " + sp + ";\n\n");
							}
						} else {
							keepSearchingBlock = false;
						}

						this.B.remove(f);
						this.buildE();
					} while (arg11.hasNext() && keepSearchingBlock);

					label89 : {
						while ((arg11 != this.t.getS().getFirst() || arg11.hasNext()) && !arg10.isEmpty()) {
							StackPair arg12 = (StackPair) arg10.pop();
							arg11 = arg12.p;
							this.t.getS().setMarker(arg11);
							this.B.remove(arg12.f);
							this.buildE();
							if (arg11.hasNext()) {
								break label89;
							}
						}

						keepSearching = false;
					}

					findP = false;
				}

				if (this.output) {
					this.outputBuffer.write(this.t.getS().getSComparator().toString() + "\n");
					this.outputBuffer.write(this.t.getS().getPComparator().toString() + "\n\n");
					this.outputBuffer.write("The number of nodes considered: " + nodesConsidered + "\n");
					this.outputBuffer.write("The number of nodes added to partial solution: " + nodesAdded + "\n");
					this.outputBuffer.write("The number of better solution found: " + betterSolutions + "\n");
				}

				if (this.dot) {
					this.dotBuffer.write("}\n");
				}

				return true;
			}
		}
	}

	private String getEStr() {
		StringBuilder sb = new StringBuilder();
		Iterator iter = this.E.iterator();

		while (iter.hasNext()) {
			Integer i = (Integer) ((Integer) iter.next());
			sb.append(i);
			if (iter.hasNext()) {
				sb.append(", ");
			}
		}

		return sb.toString();
	}

	private void buildE() {
		this.z = 0;
		this.E.clear();

		Family f;
		for (Iterator arg1 = this.B.iterator(); arg1.hasNext(); this.z += f.getCost()) {
			f = (Family) arg1.next();
			this.E.addAll(f.getRSet().getR());
		}

	}

	public TreeSet<Integer> getSolutionIDSet() {
		TreeSet b = new TreeSet();
		Iterator arg2 = this.B_hat.iterator();

		while (arg2.hasNext()) {
			Family f = (Family) arg2.next();
			b.add(Integer.valueOf(f.getId()));
		}

		return b;
	}

	public TreeSet<Family> getFullSolutionSet() {
		TreeSet b = new TreeSet(new IDFunction());
		b.addAll(this.B_hat);
		return b;
	}

	public int getSolutionCost() {
		return this.z_hat;
	}

	private String getSuccNode(Stack<StackPair> stack, boolean forLink) {
		StringBuilder sb = new StringBuilder();
		StringBuilder space = new StringBuilder();
		if (stack.isEmpty() && forLink) {
			return "Start";
		} else {
			space.append("      ");
			Iterator arg5 = stack.iterator();

			while (arg5.hasNext()) {
				StackPair sp = (StackPair) arg5.next();
				sb.append(sp.f.getId());
				space.append("      ");
			}

			return sb.toString();
		}
	}
}