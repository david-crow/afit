package edu.afit.scpsolver;

import edu.afit.scpsolver.CostFunction;
import edu.afit.scpsolver.IDFunction;
import edu.afit.scpsolver.P;
import edu.afit.scpsolver.PrimaryElementFunction;
import edu.afit.scpsolver.S;
import edu.afit.scpsolver.ast.Family;
import edu.afit.scpsolver.ast.FamilySet;
import edu.afit.scpsolver.ast.RSet;
import edu.afit.scpsolver.ast.SCP;
import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;
import java.util.HashMap;
import java.util.HashSet;
import java.util.Iterator;
import java.util.Set;
import java.util.TreeSet;

public class Tableau {
	private S s;
	private SCP f_originalSCP;
	private SCP f_newscp;
	private Comparator pc;
	private Comparator sc;
	private Set<Integer> rNotCovered;
	private Set<Family> mustBeInB;

	public Tableau(SCP scp) {
		this(scp, new PrimaryElementFunction(), new CostFunction());
	}

	public Tableau(SCP scp, Comparator sc, Comparator pc) {
		this.rNotCovered = new TreeSet();
		this.mustBeInB = new HashSet();
		this.f_originalSCP = scp;
		this.sc = sc;
		this.pc = pc;
		Iterator newSCP = scp.getRSet().getR().iterator();

		while (newSCP.hasNext()) {
			Integer oldSCP = (Integer) newSCP.next();
			boolean isCovered = false;
			Iterator arg7 = scp.getFamilySet().getFs().iterator();

			while (arg7.hasNext()) {
				Family family = (Family) arg7.next();
				if (family.getRSet().getR().contains(oldSCP)) {
					isCovered = true;
				}
			}

			if (!isCovered) {
				this.rNotCovered.add(oldSCP);
			}
		}

		if (this.rNotCovered.isEmpty()) {
			SCP oldSCP1 = new SCP(new RSet(new HashSet()), new FamilySet(new TreeSet(new IDFunction())));
			SCP newSCP1 = scp.clone();

			while (!newSCP1.equals(oldSCP1)) {
				oldSCP1 = newSCP1.clone();
				this.performReductions(newSCP1);
			}

			this.f_newscp = newSCP1;
			this.s = this.buildS(this.f_newscp);
		}

	}

	private void performReductions(SCP scp) {
		HashMap rCount = new HashMap();
		Iterator i = scp.getRSet().getR().iterator();

		Family f;
		while (i.hasNext()) {
			Integer costMatrix = (Integer) i.next();
			Iterator totalCost = scp.getFamilySet().getFs().iterator();

			while (totalCost.hasNext()) {
				f = (Family) totalCost.next();
				if (f.getRSet().getR().contains(costMatrix)) {
					if (rCount.containsKey(costMatrix)) {
						rCount.put(costMatrix, Integer.valueOf(((Integer) rCount.get(costMatrix)).intValue() + 1));
					} else {
						rCount.put(costMatrix, Integer.valueOf(1));
					}
				}
			}
		}

		Iterator arg10 = scp.getRSet().getR().iterator();

		while (true) {
			Integer arg12;
			Family arg16;
			do {
				if (!arg10.hasNext()) {
					arg10 = scp.getRSet().getR().iterator();

					while (arg10.hasNext()) {
						arg12 = (Integer) ((Integer) arg10.next());
						int arg15 = 0;
						Iterator needsCovered = this.mustBeInB.iterator();

						while (needsCovered.hasNext()) {
							arg16 = (Family) needsCovered.next();
							if (!arg16.getRSet().getR().contains(arg12)) {
								++arg15;
							}
						}

						if (((Integer) rCount.get(arg12))
								.equals(Integer.valueOf(scp.getFamilySet().getFs().size() - arg15))) {
							arg10.remove();
						}
					}

					ArrayList arg11 = new ArrayList(scp.getFamilySet().getFs());

					// heuristic
					// Collections.sort(arg11, new CostFunction());
					for (int arg13 = arg11.size() - 1; arg13 >= 0; --arg13) {
						f = (Family) arg11.get(arg13);
						if (!this.mustBeInB.contains(f)) {
							if (!f.getRSet().getR().isEmpty()
									&& this.containsAny(scp.getRSet().getR(), f.getRSet().getR())) {
								int arg17 = 0;
								TreeSet arg18 = new TreeSet(f.getRSet().getR());
								TreeSet covered = new TreeSet();

								for (int j = 0; j < arg11.size(); ++j) {
									Family fprime = (Family) arg11.get(j);
									if (f == fprime) {
										return;
									}

									if (fprime.getRSet().getR().isEmpty()
											|| !this.containsAny(scp.getRSet().getR(), f.getRSet().getR())) {
										scp.getFamilySet().getFs().remove(fprime);
										break;
									}

									if (fprime.getRSet().getR().containsAll(f.getRSet().getR())) {
										arg17 = fprime.getCost();
										arg18.clear();
									} else if (!arg18.isEmpty()) {
										arg17 += this.willCover(fprime, arg18, covered);
									}

									if (arg17 > f.getCost()) {
										break;
									}

									if (arg18.isEmpty()) {
										scp.getFamilySet().getFs().remove(f);
										break;
									}
								}
							} else {
								scp.getFamilySet().getFs().remove(f);
							}
						}
					}

					return;
				}

				arg12 = (Integer) ((Integer) arg10.next());
			} while (!((Integer) rCount.get(arg12)).equals(Integer.valueOf(1)));

			Iterator arg14 = scp.getFamilySet().getFs().iterator();

			while (arg14.hasNext()) {
				arg16 = (Family) ((Family) arg14.next());
				if (arg16.getRSet().getR().contains(arg12) && !this.mustBeInB.contains(arg16)) {
					this.mustBeInB.add(arg16);
				}
			}
		}
	}

	private S buildS(SCP scp) {
		int rSize = scp.getRSet().getR().size();
		S newS = new S(rSize, this.sc);
		Iterator arg4 = scp.getRSet().getR().iterator();

		while (arg4.hasNext()) {
			Integer i = (Integer) arg4.next();
			P p = new P(i, this.pc);
			Iterator arg7 = scp.getFamilySet().getFs().iterator();

			while (arg7.hasNext()) {
				Family family = (Family) arg7.next();
				if (family.getRSet().getR().contains(i)) {
					p.addFamily(family);
				}
			}

			newS.addBlock(p);
		}

		return newS;
	}

	private int willCover(Family f, Set<Integer> uncovered, Set<Integer> covered) {
		int cost = 0;
		if (f.getRSet().getR().containsAll(uncovered)) {
			cost = f.getCost();
			covered.addAll(uncovered);
			uncovered.clear();
		} else {
			Iterator iter = uncovered.iterator();

			while (iter.hasNext()) {
				Integer i = (Integer) ((Integer) iter.next());
				if (f.getRSet().getR().contains(i)) {
					cost = f.getCost();
					covered.add(i);
					iter.remove();
				}
			}
		}

		return cost;
	}

	private boolean containsAny(Set<Integer> s1, Set<Integer> s2) {
		Iterator arg3 = s2.iterator();

		while (arg3.hasNext()) {
			Integer integer = (Integer) arg3.next();
			if (s1.contains(integer)) {
				return true;
			}
		}

		return false;
	}

	public Set<Integer> getRNotCovered() {
		return this.rNotCovered;
	}

	public S getS() {
		return this.s;
	}

	public Set<Family> getMustBeInBSet() {
		return this.mustBeInB;
	}

	public String toString() {
		StringBuilder sb = new StringBuilder();
		sb.append(this.s.printLine() + "\n");
		sb.append(this.s.printSHeader() + "\n");
		sb.append(this.s.printLine() + "\n");
		sb.append(this.s.printPHeader() + "\n");
		sb.append(this.s.printLine() + "\n");
		sb.append(this.s.printPRows(this.s.getR()) + "\n");
		sb.append(this.s.printLine() + "\n");
		sb.append(this.s.printCost() + "\n");
		sb.append(this.s.printLine() + "\n");
		return sb.toString();
	}

	public String printOriginalSCP() {
		StringBuilder sb = new StringBuilder();
		S ns = this.buildS(this.f_originalSCP);
		sb.append(ns.printLine() + "\n");
		sb.append(ns.printSHeader() + "\n");
		sb.append(ns.printLine() + "\n");
		sb.append(ns.printPHeader() + "\n");
		sb.append(ns.printLine() + "\n");
		sb.append(ns.printPRows(new ArrayList(this.f_originalSCP.getRSet().getR())) + "\n");
		sb.append(ns.printLine() + "\n");
		sb.append(ns.printCost() + "\n");
		sb.append(ns.printLine() + "\n");
		return sb.toString();
	}

	public void sortBlocks(Comparator<P> c) {
		this.s.sort(c);
	}

	public void sortFamilies(Comparator<Family> c) {
		Iterator arg2 = this.s.getBlocks().iterator();

		while (arg2.hasNext()) {
			P p = (P) arg2.next();
			p.sort(c);
		}

	}

	public ArrayList<Integer> getR() {
		return this.s.getR();
	}
}