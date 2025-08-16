#pragma once
#include <FL/Fl.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Text_Display.H>
#include <FL/Fl_Group.H>
#include <FL/Fl_Multiline_Output.H>
#include <vector>
#include <string>
#include <set>
#include "StockData.h"
#include "StockAPI.h"
#include "LineChart.h"
#include "Portfolio.h"

class AppMainWindow : public Fl_Window {
private:
    // UI Components - Sidebar
    Fl_Group* sidebarGroup;
    Fl_Button* tradeBtn;
    Fl_Button* wishlistBtn;
    Fl_Button* portfolioBtn;
    Fl_Button* logoutBtn;
    Fl_Button* deleteAccountBtn;
    Fl_Button* analyzeViewBtn;
    Fl_Box* welcomeLabel;

    // UI Components - Main Views
    Fl_Group* tradeGroup;
    Fl_Group* wishlistGroup;
    Fl_Group* portfolioGroup;
    Fl_Group* analyzeGroup;

    // UI Components - Trade Page
    Fl_Input* stockSymbolInput;
    Fl_Button* searchButton;
    Fl_Button* addButton;
    Fl_Button* buyButton;
    Fl_Button* sellButton;
    Fl_Button* analyzeButton;
    Fl_Input* quantityInput;
    Fl_Text_Display* stockOutputDisplay;
    Fl_Text_Buffer* stockTextBuffer;
    LineChart* lineChart;

    // UI Components - Wishlist Page
    Fl_Multiline_Output* wishlistOutput;
    Fl_Button* removeStockBtn;

    // UI Components - Portfolio Page
    Fl_Multiline_Output* portfolioOutput;

    // UI Components - Analyze Page
    Fl_Multiline_Output* analyzeOutput;

    // Data Members
    Portfolio portfolio;
    std::vector<StockData> wishlist;
    std::set<int> selectedStockIndices;
    std::string currentUser;

    // Helper Methods - UI Creation
    void createSidebar(int sidebarWidth, int height);
    void createTradePage(int sidebarWidth, int width, int height, int padding);
    void createWishlistPage(int sidebarWidth, int width, int height);
    void createPortfolioPage(int sidebarWidth, int width, int height);
    void createAnalyzePage(int sidebarWidth, int width, int height);

    // Helper Methods - Data Management
    std::string getPortfolioFilename() const { return currentUser + "_portfolio.txt"; }
    std::string getWishlistFilename() const { return currentUser + "_wishlist.txt"; }
    void fetchStockAndDisplay();
    void switchTo(Fl_Group* target);
    void updateWishlistTable();
    void updatePortfolioDisplay();
    void updateCandlestickChart(const std::string& symbol);
    void saveWishlistToFile();
    void loadWishlistFromFile();
    void removeSelectedStocks();
    void toggleStockSelection(int index);
    void executeTrade(bool isBuy);
    void deleteUserAccount();
    void showDeletionMessage();
    void logoutUser();
    bool validateUserPassword(const std::string& username, const std::string& password);
    void updateAnalyzeDisplay(const std::string& focusSymbol = "");

    // Callback Methods
    static void onSearchClicked(Fl_Widget*, void*);
    static void onAddClicked(Fl_Widget*, void*);
    static void onBuyClicked(Fl_Widget*, void*);
    static void onSellClicked(Fl_Widget*, void*);
    static void onTradeBtnClicked(Fl_Widget*, void*);
    static void onWishlistBtnClicked(Fl_Widget*, void*);
    static void onPortfolioBtnClicked(Fl_Widget*, void*);
    static void onRemoveStockClicked(Fl_Widget*, void*);
    static void onWishlistOutputClicked(Fl_Widget*, void*);
    static void onDeleteAccountClicked(Fl_Widget*, void*);
    static void onLogoutClicked(Fl_Widget*, void*);
    static void onConfirmDeleteClicked(Fl_Widget*, void*);
    static void onAnalyzeClicked(Fl_Widget*, void*);
    static void onAnalyzeViewBtnClicked(Fl_Widget*, void*);

public:
    AppMainWindow(int w, int h, const char* title);
    ~AppMainWindow();
    void setCurrentUser(const std::string& user);
};
