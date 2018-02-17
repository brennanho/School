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