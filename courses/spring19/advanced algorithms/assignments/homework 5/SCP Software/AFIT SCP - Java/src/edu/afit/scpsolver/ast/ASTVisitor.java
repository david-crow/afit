package edu.afit.scpsolver.ast;

import edu.afit.scpsolver.ast.Family;
import edu.afit.scpsolver.ast.FamilySet;
import edu.afit.scpsolver.ast.RSet;
import edu.afit.scpsolver.ast.SCP;

public class ASTVisitor {
	public boolean visit(Family node) {
		return true;
	}

	public boolean visit(FamilySet node) {
		return true;
	}

	public boolean visit(RSet node) {
		return true;
	}

	public boolean visit(SCP node) {
		return true;
	}
}