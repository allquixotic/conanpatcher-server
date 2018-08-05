#include <Wt/WApplication.h>
#include <Wt/WBreak.h>
#include <Wt/WContainerWidget.h>
#include <Wt/WLineEdit.h>
#include <Wt/WPushButton.h>
#include <Wt/WText.h>
#include <Wt/WBootstrapTheme.h>
#include <Wt/WServer.h>
#include <Wt/Auth/AuthWidget.h>
#include <Wt/Auth/PasswordService.h>

#include "Session.h"

class PatcherApplication : public Wt::WApplication
{
public:
    PatcherApplication(const Wt::WEnvironment& env);
    void authEvent();

private:
    Wt::WLineEdit *nameEdit_;
    Wt::WText *greeting_;
    Session session_;
};

PatcherApplication::PatcherApplication(const Wt::WEnvironment& env)
    : Wt::WApplication(env),
      session_(appRoot() + "auth.db")
{
    setTitle("Patcher Login");

    session_.login().changed().connect(this, &PatcherApplication::authEvent);

    root()->addStyleClass("container");
    //this->setCssTheme("polished");
    setTheme(std::make_shared<Wt::WBootstrapTheme>());

    useStyleSheet("css/style.css");

    std::unique_ptr<Wt::Auth::AuthWidget> authWidget
            = Wt::cpp14::make_unique<Wt::Auth::AuthWidget>(Session::auth(), session_.users(), session_.login());

    authWidget->model()->addPasswordAuth(&Session::passwordAuth());
    //authWidget->model()->addOAuth(Session::oAuth());
    authWidget->setRegistrationEnabled(true);

    authWidget->processEnvironment();

    root()->addWidget(std::move(authWidget));

/*
    root()->addNew<Wt::WText>("Your name, please? ");
    nameEdit_ = root()->addNew<Wt::WLineEdit>();
    Wt::WPushButton *button = root()->addNew<Wt::WPushButton>("Greet me.");
    root()->addNew<Wt::WBreak>();
    greeting_ = root()->addNew<Wt::WText>();
    auto greet = [this]{
      greeting_->setText("Patcher there, " + nameEdit_->text());
    };
    button->clicked().connect(greet);
    */

}

void PatcherApplication::authEvent() {
    if (session_.login().loggedIn()) {
        const Wt::Auth::User& u = session_.login().user();
        log("notice")
                << "User " << u.id()
                << " (" << u.identity(Wt::Auth::Identity::LoginName) << ")"
                << " logged in.";
    } else
        log("notice") << "User logged out.";
}

std::unique_ptr<Wt::WApplication> createApplication(const Wt::WEnvironment& env)
{
    return std::make_unique<PatcherApplication>(env);
}

int main(int argc, char **argv)
{
    try {
        Wt::WServer server{argc, argv, WTHTTP_CONFIGURATION};

        server.addEntryPoint(Wt::EntryPointType::Application, createApplication);

        Session::configureAuth();

        server.run();
    } catch (Wt::WServer::Exception& e) {
        std::cerr << e.what() << std::endl;
    } catch (Wt::Dbo::Exception &e) {
        std::cerr << "Dbo exception: " << e.what() << std::endl;
    } catch (std::exception &e) {
        std::cerr << "exception: " << e.what() << std::endl;
    }

    /*
    return Wt::WRun(argc, argv, [](const Wt::WEnvironment& env) {
      return std::make_unique<PatcherApplication>(env);
    });
     */
}
