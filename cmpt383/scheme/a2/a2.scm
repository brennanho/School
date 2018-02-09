;q1
(define singleton?
	(lambda (x)
		(cond 
			((not (list? x)) 
				#f)
			((null? (cdr x))
				 #t)
			(else 
				#f)
		)
	)
)

;q2
(define my-make-list
	(lambda (n x)
		(cond
			(( = n 0) 
				'())
			(else 
				(cons x (my-make-list (- n 1) x))
			)
		)
	)	
)

;q3 (uses my-append as a helper from q8)
(define my-iota
	(lambda (n)
		(cond
			((= n 0) 
				'())
			(else
				(my-append (my-iota (- n 1)) (list(- n 1)) )
			)
		)
	)
)

;Q4
(define my-len
	(lambda (lst)
		(cond
			((null? lst)
				 0)
			(else
				(+ 1 (my-len (cdr lst)))
			)
		)
	)
)

;q5
(define nth
	(lambda (lst i)
		(cond 
			((null? lst) (error "bad index"))
			((= i 0) 
				(car lst))
			(else
				(nth (cdr lst) (- i 1))
			)
		)
	)
)

;q6
(define my-last 
	(lambda (lst)
		(cond
			((null? lst) (error "my-last: empty list"))
			((= (my-len lst) 1) 
				(car lst))
			(else
				(my-last (cdr lst))
			)
		)
	)
)

;q7
(define my-filter
	(lambda(pred lst)
		(cond
			((null? lst) 
				lst)
			((pred (car lst)) 
				(cons (car lst) (my-filter pred (cdr lst))))
			(else
				(my-filter pred (cdr lst))
			)
		)
	)
)

;q8 Helper (removes the last element in a list, i.e. opposite of cdr)
(define my-remove-last
	(lambda (lst)
	    (cond 
	    	((null? lst) 
	    		'())
	    	((null? (cdr lst)) 
	    		'())
	    	(else
	        	(cons (car lst) (my-remove-last (cdr lst)))
	        )
    	)
    )
)

;q8
(define my-append
	(lambda(A B)
		(cond
			((null? A) B)
			(else
				(my-append (my-remove-last A) (cons (my-last A) B))
			)
		)
	)
)

;q9
(define append-all
	(lambda (lol)
		(cond
			((null? lol) lol)
			(else
				(my-append (car lol) (append-all (cdr lol)))
			)
		)
	)
)

;q10 (quick sort implementation)
(define (my-sort lst) 
   (cond 
   	  ((null? lst) lst)
      (else
      	(let ((x (car lst)) ; Pivot element
        	(lstPart (cdr lst)))
         	(append 
         		 (my-sort
                 	(my-filter (lambda (y) (< y x)) ; Left side partition
                        lstPart))

                 (list x)

                 (my-sort
                    (my-filter (lambda (y) (>= y x)) ; Right side partition
                        lstPart)
                 )
            )
        )
      )
    )
)

(define insert-all-bits
	(lambda(x lst)
		(cons x lst) lst
	)
)

;q11
(define all-bits
	(lambda (n)
		(cond
			((= 0 n) '())
			((= 1 n) '((0) (1)))
			(else
				(my-append '(0) (car(list(all-bits (- n 1)))))
			)
		)
	)
)
