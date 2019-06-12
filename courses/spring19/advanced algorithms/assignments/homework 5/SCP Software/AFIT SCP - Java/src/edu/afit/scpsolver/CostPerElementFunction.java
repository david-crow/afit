package edu.afit.scpsolver;

import edu.afit.scpsolver.ast.Family;
import java.util.Comparator;

public class CostPerElementFunction implements Comparator<Family> {
	public int compare(Family f1, Family f2) {
		return f1.getCostPerElement() - f2.getCostPerElement();
	}

	public String toString() {
		return "CostPerElementFunction: Sorted families (j) from least cost per element to greatest cost per element.";
	}
}