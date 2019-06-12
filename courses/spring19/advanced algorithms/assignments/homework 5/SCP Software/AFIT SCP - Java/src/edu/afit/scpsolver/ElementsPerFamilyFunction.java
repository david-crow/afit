package edu.afit.scpsolver;

import edu.afit.scpsolver.ast.Family;
import java.util.Comparator;

public class ElementsPerFamilyFunction implements Comparator<Family> {
	public int compare(Family f1, Family f2) {
		return f2.getRSet().getR().size() - f1.getRSet().getR().size();
	}

	public String toString() {
		return "ElementsPerFamilyFunction: Sorted families (j) from greatest cardinality to least cardinality.";
	}
}