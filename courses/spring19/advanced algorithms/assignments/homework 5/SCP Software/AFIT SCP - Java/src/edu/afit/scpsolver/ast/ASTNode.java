package edu.afit.scpsolver.ast;

import edu.afit.scpsolver.ast.ASTVisitor;

public abstract class ASTNode {
	public abstract void accept(ASTVisitor arg0);
}