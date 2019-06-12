package edu.afit.scpsolver;

import edu.afit.scpsolver.ast.Family;
import java.util.Comparator;

public class CostFunction implements Comparator<Family> {
	public int compare(Family f1, Family f2) {
		return f1.getCost() - f2.getCost();
	}

	public String toString() {
		return "CostFunction: Sorted families (j) from least cost to greatest cost.";
	}
}