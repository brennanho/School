(load "myeval.scm")

;HELPER - Simplifies an expression that contains no subexpressions e.g. (a + 0)
(define simplify-base
  (lambda (expr)
    (cond
      ((and (equal? (car expr) 'inc) (number? (cadr expr)))
        (+ (cadr expr) 1))
      ((and (equal? (car expr) 'dec) (number? (cadr expr)))
        (- (cadr expr) 1))
      ((and (equal? (car expr) 'inc) (not (list? (cadr expr))))
        (list (cadr expr) '+ 1))
      ((and (equal? (car expr) 'dec) (not (list? (cadr expr))))
        (list (cadr expr) '- 1))
      ((and (equal? (cadr expr) '+) (equal? (car expr) 0))
           (last-of-expr expr))
      ((and (equal? (cadr expr) '+) (equal? (last-of-expr expr) 0))
           (car expr))
      ((and (equal? (cadr expr) '*) (equal? (car expr) 1))
           (last-of-expr expr))
      ((and (equal? (cadr expr) '*) (equal? (last-of-expr expr) 1))
           (car expr))
      ((and (equal? (cadr expr) '*) (equal? (car expr) 0))
           0)
      ((and (equal? (cadr expr) '*) (equal? (last-of-expr expr) 0))
           0)
      ((and (equal? (cadr expr) '/) (equal? (last-of-expr expr) 1))
           (car expr))
      ((and (equal? (cadr expr) '-) (equal? (last-of-expr expr) 0))
           (car expr))
      ((and (equal? (cadr expr) '-) (equal? (last-of-expr expr) (car expr)))
           0)
      ((and (equal? (cadr expr) '**) (equal? (last-of-expr expr) 0))
           1)
      ((and (equal? (cadr expr) '**) (equal? (last-of-expr expr) 1))
           (car expr))
      ((and (equal? (cadr expr) '**) (equal? (car expr) 1))
           1)
      (else
        expr  
      )
    )  
  )  
)

;Required for part 3
(define simplify
  (lambda (expr)
    (cond
      ;No sub expression, evaluate expression as is
      ((no-subexpr? expr)
        (simplify-base expr)) 

      ;Expression contains a subexpression but is of form dec or inc e.g. (inc (3 + x))
      ((= (length expr) 2) 
          (simplify-base (list (car expr) (simplify (cadr expr)))))

      ;If both operands are subexpressions e.g. ((3 + x) + (3 * y))
      ((and (list? (car expr)) (list? (last-of-expr expr))) 
          (simplify-base (list (simplify (car expr)) (cadr expr) (simplify (last-of-expr expr)))))
      
      ;If left operand is a subexpression e.g. ((3 + x) * 2)
      ((list? (car expr))
          (simplify-base (list (simplify (car expr)) (cadr expr) (last-of-expr expr))))
      
      ;Right operand is a subexpression e.g. (2 * (3 + x))
      (else
          (simplify-base (list (car expr) (cadr expr) (simplify (last-of-expr expr))))
      )
    )
  )
)