(define make-empty-env
    (lambda () 
      '()
    )
)

(define v-in-env
  (lambda (env v)
    (cond
      ((null? env) 
            #f)
      ((eq? (car (car env)) v) 
            #t)
      (else
        (v-in-env (cdr env) v)  
      )
    )
  )  
)

(define replace-v-env
  (lambda (env v val)
    (cond
      ((null? env) env)
      ((eq? (car (car env)) v) 
        (cons (list v val) (replace-v-env (cdr env) v val)))
      (else
        (cons (car env) (replace-v-env (cdr env) v val))
      )
    )
  )  
)

(define apply-env
  (lambda (env v)
    (cond
      ((null? env)
        (error "apply-env: empty environment"))
      ((eq? v (car (car env)))
        (car (cdr (car env))))
      (else
        (apply-env (cdr env) v)
      )
    )
  )
)

(define extend-env
    (lambda (v val env)
      (cond
        ((not (v-in-env env v))
            (cons (list v val) env))
        (else
            (replace-v-env env v val)
        )
      )
    )
)

(define test-env
    (extend-env 'a 1
        (extend-env 'b 2
            (extend-env 'c 3
                (extend-env 'b 4
                    (make-empty-env)))))
)

;part 2

(define last-of-expr
  (lambda(expr)
    (car (cdr (cdr expr)))
  )
)

(define no-subexpr
  (lambda (expr)
    (cond
      ( (and (not (list? (car expr))) (not (list? (cadr expr))) (not (list? (last-of-expr expr))))
        #t)
      (else
        #f)
    )
  )
)

(define eval-expr
    (lambda (expr)
      (cond
        ((eq? (car expr) 'inc)
              (+ (cadr expr) 1))
        ((eq? (car expr) 'dec)
              (- (cadr expr) 1))
        ((eq? (cadr expr) '+) 
          (+ (car expr) (last-of-expr expr)))
        ((eq? (cadr expr) '-) 
          (- (car expr) (last-of-expr expr)))
        ((eq? (cadr expr) '*) 
          (* (car expr) (last-of-expr expr)))
        ((eq? (cadr expr) '/) 
          (/ (car expr) (last-of-expr expr)))
        ((eq? (cadr expr) '**) 
          (expt (car expr) (last-of-expr expr)))
      )
    )
)

(define eval-nested-3
  (lambda (expr)
    (cond
        ((no-subexpr expr)
          (eval-expr expr))
        ((and (list? (car expr)) (list? (last-of-expr expr)))
          (eval-expr (list (eval-nested-3 (car expr)) (cadr expr) (eval-nested-3 (last-of-expr expr)))))
        ((list? (car expr))
           (eval-expr (list (eval-nested-3 (car expr)) (cadr expr) (last-of-expr expr))))
        (else
          (eval-expr (list (car expr) (cadr expr) (eval-nested-3 (last-of-expr expr))))
        )
    )
  )
)






