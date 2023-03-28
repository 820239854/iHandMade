;;; whitebox.el --- Description -*- lexical-binding: t; -*-
;;
;; Copyright (C) 2021 WhiteBox Systems
;;
;; Author: Quentin Carver <qgcarver@protonmail.com>
;; Maintainer: Andrew Reece <john@doe.com>
;; Created: September 20, 2021
;; Modified: October 22, 2021
;; Version: 0.0.2
;; Keywords: c games hardware languages lisp processes tools whitebox
;; Homepage: https://whitebox.systems
;; Package-Requires: ((emacs "25.1"))
;;
;; This file is not part of GNU Emacs.
;;
;;; Commentary:
;;  Use (load-file <path-to-whitebox.el>) to use the plugin.
;;
;;  Description
;;  This is the provided Emacs plugin for WhiteBox.
;;
;;; Code:

(require 'json)

(defgroup whitebox nil
  "The WhiteBox plugin."
  :group 'external)
(defcustom whitebox-host "localhost"
  "Host used by the another Emacs server."
  :type 'string)
(defcustom whitebox-port 19013
  "Port used by the another Emacs server."
  :type 'integer)
(defcustom whitebox-editor-code-file 1
  "The code for the 'file' editor's socket."
  :type 'integer)
(defcustom whitebox-editor-code-vim 4
  "The code for vim's socket."
  :type 'integer)

(defun whitebox-status nil
  "Return non-nil if whitebox is connected."
  (process-status "whitebox"))

(defvar whitebox-mode nil
  "Toggle whitebox-mode.")

(defun whitebox-mode (&optional ARG)
  "The WhiteBox mode, ARG will override the toggle."
  (interactive (list 'toggle))
  (setq whitebox-mode
        (if (eq ARG 'toggle)
            (not whitebox-mode)
          (> ARG 0)))
  (if whitebox-mode
      (cond ((eq (whitebox-status) 'nil)
             (progn (whitebox-connect)
                    (unless (eq (whitebox-status) 'open)
                      (setq whitebox-mode 'nil))))
            (t (message (format "WhiteBox status: %s (Unexpected)" (whitebox-status)))))
    (cond ((eq (whitebox-status) 'open)
           (progn (whitebox-disconnect)
                  (if (eq (whitebox-status) 'nil)
                      (setq whitebox-mode 'nil)
                    (message "WhiteBox failed to disconnect."))))
          (t (progn (setq whitebox-mode 'nil)
                    (message (format (concat
                                      "WhiteBox status: %s (Unexpected)\n"
                                      "whitebox-mode turned off\n") (whitebox-status))))))))

(defun whitebox-plugin-hook nil
  "The WhiteBox plugin hook."
  (if (and
       whitebox-mode
       (buffer-file-name))
      (whitebox-send-data)))

(defun whitebox-connect nil
  "Connect to a WhiteBox process."
  (unless (whitebox-status)
    (ignore-errors (make-network-process :name "whitebox" :buffer "*whitebox*" :family 'ipv4
                                         :host whitebox-host :service whitebox-port))
    (message (whitebox-check)))
  (when (whitebox-status)
    (add-hook 'post-command-hook 'whitebox-plugin-hook)))

(defun whitebox-disconnect nil
  "Disconnect from WhiteBox."
  (interactive)
  (when (whitebox-status) (delete-process "whitebox"))
  (message (whitebox-check))
  (unless (whitebox-status) (remove-hook 'post-command-hook 'whitebox-plugin-hook))
  (setq whitebox-mode 'nil))

(defun whitebox-send-data nil
  "Send json data to whitebox."
  (if (whitebox-status)
      (process-send-string "whitebox" (whitebox-plugin-json))
    (whitebox-disconnect)))


(defun whitebox-plugin-json nil
  "Get cursor and mark from Emacs."
  ;; Figure out how alists->json should
  ;; work. Ostensibly we only really need
  ;; to send the simple command shown in
  ;; the vim plugin. So maybe we should
  ;; hard-code it in for now and worry
  ;; about making things programmatic
  ;; and data-centric later. Also we will
  ;; need a proper mark concept for ranges
  ;; which means learning more vanilla
  ;; style emacs ;-;
  (let ((current-col (current-column))
        (current-line (count-lines 1 (point)))
        (dirty (if (buffer-modified-p) ["unsaved"] []))
        (path (buffer-file-name)))
    (json-encode-alist  `(("editor" . "Emacs")
                          ("path" . ,path)
                          ("selection" . [(("line" . ,current-line)
                                           ("column" . ,current-col))
                                          ;; find mark fns to allow for range
                                          (("line" . ,current-line)
                                           ("column" . ,current-col))])
                          ("dirty" . ,dirty)))))

(defun whitebox-connection-check nil
  "Check the connection status of WhiteBox."
  (interactive)
  (message (whitebox-check)))

(defun whitebox-check nil
  "Print the connection status of the WhiteBox plugin."
  (let ((s (whitebox-status)))
  ;; The returned value is one of the following symbols:
  ;; run  -- for a process that is running.
  ;; stop -- for a process stopped but continuable.
  ;; exit -- for a process that has exited.
  ;; signal -- for a process that has got a fatal signal.
  ;; open -- for a network stream connection that is open.
  ;; listen -- for a network stream server that is listening.
  ;; closed -- for a network stream connection that is closed.
  ;; connect -- when waiting for a non-blocking connection to complete.
  ;; failed -- when a non-blocking connection has failed.
  ;; nil -- if arg is a process name and no such process exists.
  ;; PROCESS may be a process, a buffer, the name of a process, or
  ;; nil, indicating the current buffer's process.
    (cond ((eq s 'open) "WhiteBox connection: Open")
          ((eq s 'nil)  "WhiteBox connection: Closed")
          (t (format "WhiteBox socket status: %s (Unexepected)" 's))))  )

(provide 'whitebox)
;;; whitebox.el ends here
