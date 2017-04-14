;; uncomment to fix helm-ls-git in emacs 24.3
;;(require 'eieio)

(require 'package)
(add-to-list 'package-archives '("melpa" . "http://melpa.org/packages/"))
(package-initialize)

(add-to-list 'custom-theme-load-path "~/.emacs.d/themes/")

(custom-set-variables
 ;; custom-set-variables was added by Custom.
 ;; If you edit it by hand, you could mess it up, so be careful.
 ;; Your init file should contain only one such instance.
 ;; If there is more than one, they won't work right.
 '(before-save-hook (quote (delete-trailing-whitespace)))
 '(c++-mode-hook (quote (irony-mode)))
 '(c-basic-offset 4)
 '(c-mode-common-hook
   (quote
    ((lambda nil
       (local-set-key
        (kbd "RET")
        (quote newline-and-indent))))))
 '(c-mode-hook (quote (irony-mode)))
 '(column-number-mode t)
 '(company-idle-delay 0.1)
 '(cursor-type (quote bar))
 '(custom-enabled-themes (quote (monokai)))
 '(custom-safe-themes
   (quote
    ("2288379e9e9fe0ef64e49527693cbbe8918e1d5fc1040ca66fe04f0a77f7fb84" default)))
 '(electric-pair-mode t)
 '(flycheck-clang-language-standard "c++11")
 '(flycheck-gcc-language-standard "c++11")
 '(global-company-mode t)
 '(global-flycheck-mode t)
 '(global-hl-line-mode t)
 '(global-linum-mode t)
 '(google-this-mode t)
 '(helm-mode t)
 '(indent-tabs-mode nil)
 '(inhibit-startup-screen t)
 '(line-number-mode t)
 '(menu-bar-mode nil)
 '(org-babel-load-languages (quote ((emacs-lisp . t) (C . t))))
 '(scroll-conservatively 1)
 '(show-paren-mode t)
 '(tool-bar-mode nil))
(custom-set-faces
 ;; custom-set-faces was added by Custom.
 ;; If you edit it by hand, you could mess it up, so be careful.
 ;; Your init file should contain only one such instance.
 ;; If there is more than one, they won't work right.
 '(helm-ff-directory ((t (:foreground "‪#‎66D9EF‬"))))
 '(helm-selection ((t (:inherit bold :background "gray25" :underline nil))))
 '(hl-line ((t (:background "#404040"))))
 '(region ((t (:inherit highlight :background "#330033")))))

;; replace the `completion-at-point' and `complete-symbol' bindings in
;; irony-mode's buffers by irony-mode's function
(defun my-irony-mode-hook ()
 (define-key irony-mode-map [remap completion-at-point]
  'irony-completion-at-point-async)
 (define-key irony-mode-map [remap complete-symbol]
  'irony-completion-at-point-async))
(add-hook 'irony-mode-hook 'my-irony-mode-hook)
(add-hook 'irony-mode-hook 'irony-cdb-autosetup-compile-options)

;; Add company-irony to company backends
(eval-after-load 'company
 '(add-to-list 'company-backends 'company-irony))

;;
(require 'helm-config)
(global-set-key (kbd "M-x") 'helm-M-x)
(global-set-key (kbd "C-x C-d") 'helm-browse-project)
(global-set-key (kbd "C-x C-f") 'helm-find-files)
(define-key helm-map (kbd "TAB") 'helm-execute-persistent-action)

;;
(require 'neotree)
(global-set-key [f8] 'neotree-toggle)

;;
(require 'multiple-cursors)
(global-set-key (kbd "C-S-c C-S-c") 'mc/edit-lines)
(global-set-key (kbd "C->") 'mc/mark-next-like-this)
(global-set-key (kbd "C-<") 'mc/mark-previous-like-this)
(global-set-key (kbd "C-c C-<") 'mc/mark-all-like-this)
(global-set-key (kbd "C-S-<mouse-1>") 'mc/add-cursor-on-click)

;;
(require 'org)

;; easily switch back and forth between term-char-mode and term-line-mode
;; from https://joelmccracken.github.io/entries/switching-between-term-mode-and-line-mode-in-emacs-term/
(require 'term)

(defun jnm/term-toggle-mode ()
  "Toggles term between line mode and char mode"
  (interactive)
  (if (term-in-line-mode)
      (term-char-mode)
    (term-line-mode)))

(define-key term-mode-map (kbd "C-c C-j") 'jnm/term-toggle-mode)
(define-key term-mode-map (kbd "C-c C-k") 'jnm/term-toggle-mode)

(define-key term-raw-map (kbd "C-c C-j") 'jnm/term-toggle-mode)
(define-key term-raw-map (kbd "C-c C-k") 'jnm/term-toggle-mode)

;;
(setq sml/theme 'powerline)
(sml/setup)

;;
(put 'downcase-region 'disabled nil)
(put 'upcase-region 'disabled nil)

;;
(defun eval-and-replace ()
  "Replace the preceding sexp with its value."
  (interactive)
  (backward-kill-sexp)
  (condition-case nil
      (prin1 (eval (read (current-kill 0)))
             (current-buffer))
      (error (message "Invalid expression")
             (insert (current-kill 0)))))
(global-set-key (kbd "C-c e") 'eval-and-replace)
