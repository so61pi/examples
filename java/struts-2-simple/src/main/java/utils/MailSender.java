package utils;

import java.util.Properties;

import javax.activation.DataHandler;
import javax.activation.DataSource;
import javax.activation.FileDataSource;
import javax.mail.BodyPart;
import javax.mail.Message;
import javax.mail.MessagingException;
import javax.mail.Multipart;
import javax.mail.PasswordAuthentication;
import javax.mail.Session;
import javax.mail.Transport;
import javax.mail.internet.InternetAddress;
import javax.mail.internet.MimeBodyPart;
import javax.mail.internet.MimeMessage;
import javax.mail.internet.MimeMultipart;

public class MailSender {

    private static final String username = "Example@gmail.com";
    private static final String password = "1234567890";

    public static Status send(MailPerson from, MailPerson to, String subject,
            String what) {
        Properties props = new Properties();
        props.put("mail.smtp.auth", "true");
        props.put("mail.smtp.starttls.enable", "true");
        props.put("mail.smtp.host", "smtp.gmail.com");
        props.put("mail.smtp.port", "587");

        Session session = Session.getInstance(props,
                new javax.mail.Authenticator() {
                    @Override
                    protected PasswordAuthentication getPasswordAuthentication() {
                        return new PasswordAuthentication(username, password);
                    }
                });

        try {
            Message msg = new MimeMessage(session);
            msg.setFrom(new InternetAddress(from.email, from.name));
            msg.addRecipient(Message.RecipientType.TO, new InternetAddress(
                    to.email, to.name));
            msg.setSubject(subject);
            msg.setText(what);
            Transport.send(msg);
        }
        catch (MessagingException e) {
            ExceptionHandler.handle(e);
            return Status.MAIL_CANNOT_SEND;
        }
        catch (Exception e) {
            ExceptionHandler.handle(e);
            return Status.MAIL_CANNOT_SEND;
        }

        return Status.SUCCESS;
    }

    public static Status sendComplex(MailPerson from, MailPerson[] toList,
            MailPerson[] ccList, MailPerson[] bccList, String subject,
            String what, String attachmentFilePath) {
        Properties props = new Properties();
        Session session = Session.getDefaultInstance(props, null);

        try {
            MimeMessage message = new MimeMessage(session);
            message.setFrom(new InternetAddress(from.email, from.name));
            message.setSubject(subject);

            if (toList != null) {
                for (MailPerson p : toList) {
                    message.addRecipient(Message.RecipientType.TO,
                            new InternetAddress(p.email, p.name));
                }
            }

            if (ccList != null) {
                for (MailPerson p : ccList) {
                    message.addRecipient(Message.RecipientType.TO,
                            new InternetAddress(p.email, p.name));
                }
            }

            if (bccList != null) {
                for (MailPerson p : bccList) {
                    message.addRecipient(Message.RecipientType.TO,
                            new InternetAddress(p.email, p.name));
                }
            }

            // Create a multipart message
            Multipart multipart = new MimeMultipart();

            // Create the message part & fill the message
            BodyPart messagePart = new MimeBodyPart();
            messagePart.setText(what);

            // Set text message part
            multipart.addBodyPart(messagePart);

            // Part two is attachment
            BodyPart attachmentPart = new MimeBodyPart();
            DataSource source = new FileDataSource(attachmentFilePath);
            attachmentPart.setDataHandler(new DataHandler(source));
            attachmentPart.setFileName(attachmentFilePath);

            // Set attachment part
            multipart.addBodyPart(attachmentPart);

            // Send the complete message parts
            message.setContent(multipart);

            // Send message
            Transport.send(message);
        }
        catch (MessagingException e) {
            ExceptionHandler.handle(e);
            return Status.MAIL_CANNOT_SEND;
        }
        catch (Exception e) {
            ExceptionHandler.handle(e);
            return Status.MAIL_CANNOT_SEND;
        }

        return Status.SUCCESS;
    }
}
