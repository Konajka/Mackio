/**
 * @brief Menu backend with rendering and scrolling support.
 *
 * @author https://github.com/Konajka
 * @version 0.1 2019-05-01
 *  Base menu model definition.
 * @version 0.2 2019-05-03
 *  Base rendering.
 * @version 0.3 2019-05-05
 *  Menu items scrolling support for rendering.
 * @version 0.4 2019-05-14
 *  Added doc comments.
 * @version 0.5 2019-05-15
 *  Added controller events.
 *  Separated renderer classes.
 * @version 0.6 2019-05-17
 *  Added menu index to renderer.
 *  Added user custom tag and data pointer definition.
 * @version 0.7 2019-05-20
 *  Fixed active menu item changing to change before event is yield.
 * @version 0.8 2019-06-21
 *  Removed QMENU_USE_EXTENDED_INFO directive.
 * @version 0.9 2019-06-23
 *  Added checkable and radio item support.
 * @version 0.10 2019-06-26
 *  Added checked flag to item instantiation.
 * @version 1.0 2019-07-04
 *  Stable version.
 */

#ifndef QMENU_H
#define QMENU_H

#include <Arduino.h>
#include <String.h>

#define QMENU_ITEM_REGULAR 0
#define QMENU_ITEM_CHECKABLE 255

/**
 * @brief Menu item definition.
 */
class QMenuItem {
    private:
        /* Item identification */
        int _id;

        /* Printable caption */
        char* _caption;

        /* Custom integer data */
        int _tag = 0;

        /* Custom pointer data */
        void* _data = NULL;

        /* Group index
            0 - regular item, not groupped, not checkable
            1-254 - item is radio item, number is radio group index
            255 - item is checkable
         */
        byte _groupIndex = QMENU_ITEM_REGULAR;

        /* Radio group item or checkable item checked flag */
        bool _checked = false;

        /* Links to neighbour items */
        QMenuItem* _back = NULL;
        QMenuItem* _menu = NULL;
        QMenuItem* _prev = NULL;
        QMenuItem* _next = NULL;

    public:

        /**
         * @brief Static QMenuItem instantiating and initialization.
         * @param id Menu item identification. This could be unique integer value, but no unique
         *      test is performed.
         * @param caption Menu item caption.
         */
        static QMenuItem* create(int id, char* caption) {
            return new QMenuItem(id, caption);
        }

        /**
         * @brief Static QMenuItem instantiating and initialization.
         * @param id Menu item identification. This could be unique integer value, but no unique
         *      test is performed.
         * @param caption Menu item caption.
         * @param tag User defined integer value.
         */
        static QMenuItem* create(int id, char* caption, int tag) {
            QMenuItem* item = new QMenuItem(id, caption);
            item->setTag(tag);
            return item;
        }

        /**
         * @brief Static QMenuItem instantiating and initialization.
         * @param id Menu item identification. This could be unique integer value, but no unique
         *      test is performed.
         * @param caption Menu item caption.
         * @param data User defined data pointer.
         */
        static QMenuItem* create(int id, char* caption, void* data) {
            QMenuItem* item = new QMenuItem(id, caption);
            item->setData(data);
            return item;
        }

        /**
         * @brief Static QMenuItem instantiating and initialization.
         * @param id Menu item identification. This could be unique integer value, but no unique
         *      test is performed.
         * @param caption Menu item caption.
         * @param tag User defined integer value.
         * @param data User defined data pointer.
         */
        static QMenuItem* create(int id, char* caption, int tag, void* data) {
            QMenuItem* item = new QMenuItem(id, caption);
            item->setTag(tag);
            item->setData(data);
            return item;
        }

        /**
         * @brief Static QMenuItem instantiating and initialization.
         * @param id Menu item identification. This could be unique integer value, but no unique
         *      test is performed.
         * @param caption Menu item caption.
         * @param groupIndex Item's group index to be set.
         * @param checked Set to true to initialize item as checked or false to not checked.
         */
        static QMenuItem* createRadio(int id, char* caption, byte groupIndex, bool checked) {
            QMenuItem* item = new QMenuItem(id, caption);
            item->setGroupIndex(groupIndex);
            item->setChecked(checked);
            return item;
        }

        /**
         * @brief Static QMenuItem instantiating and initialization.
         * @param id Menu item identification. This could be unique integer value, but no unique
         *      test is performed.
         * @param caption Menu item caption.
         * @param groupIndex Item's group index to be set.
         * @param checked Set to true to initialize item as checked or false to not checked.
         * @param tag User defined integer value.
         * @param data User defined data pointer.
         */
        static QMenuItem* createRadio(int id, char* caption, byte groupIndex, bool checked, int tag, void* data) {
            QMenuItem* item = new QMenuItem(id, caption);
            item->setGroupIndex(groupIndex);
            item->setChecked(checked);
            item->setTag(tag);
            item->setData(data);
            return item;
        }

                /**
         * @brief Static QMenuItem instantiating and initialization.
         * @param id Menu item identification. This could be unique integer value, but no unique
         *      test is performed.
         * @param caption Menu item caption.
         * @param checked Set to true to initialize item as checked or false to not checked.
         */
        static QMenuItem* createCheckable(int id, char* caption, bool checked) {
            QMenuItem* item = new QMenuItem(id, caption);
            item->setGroupIndex(QMENU_ITEM_CHECKABLE);
            item->setChecked(checked);
            return item;
        }

        /**
         * @brief Static QMenuItem instantiating and initialization.
         * @param id Menu item identification. This could be unique integer value, but no unique
         *      test is performed.
         * @param caption Menu item caption.
         * @param checked Set to true to initialize item as checked or false to not checked.
         * @param tag User defined integer value.
         * @param data User defined data pointer.
         */
        static QMenuItem* createCheckable(int id, char* caption, bool checked, int tag, void* data) {
            QMenuItem* item = new QMenuItem(id, caption);
            item->setGroupIndex(QMENU_ITEM_CHECKABLE);
            item->setChecked(checked);
            item->setTag(tag);
            item->setData(data);
            return item;
        }

        /**
         * @brief Creates new QMenuItem instance.
         * @param id Menu item identification.
         * @param caption Menu item caption.
         */
        QMenuItem(int id, char* caption) {
            _id = id;
            _caption = caption;
        }

        /**
         * @brief Gets menu item identification.
         * @return Returns menu item identification.
         */
        int getId() {
            return this->_id;
        }

        /**
         * @brief Gets menu item caption.
         * @return Returns menu item caption.
         */
        char* getCaption() {
            return this->_caption;
        }

        /**
         * @brief Gets user custom integer value.
         * @return Returns tag value.
         */
        int getTag() {
            return _tag;
        }

        /**
         * @brief Sets user custom integer value.
         */
        void setTag(int tag) {
            _tag = tag;
        }

        /**
         * @brief Gets user defined data pointer.
         * @return Returns void pointer to custom data.
         */
        void* getData() {
            return _data;
        }

        /**
         * @brief Sets user defined data pointer.
         */
        void setData(void* data) {
            _data = data;
        }

        /**
         * @brief Gets if this menu item is flagged as checked. This flag does not depend on item's
         * group index (or item type).
         * @return Returns true if this item is checked or false if not.
         */
        bool isChecked() {
            return _checked;
        }

        /**
         * @brief Sets this menu item checked flag. This flag does not depend on item's group index
         * (or item type) and does not affects other items in same group defined via
         * setGroupIndex().
         * @param checked Set true to flag this item checked or false to unchecked.
         */
        bool setChecked(bool checked) {
            _checked = checked;
        }

        /**
         * @brief Gets item's group index.
         * @return Returns group index number. Number meaning is:
         *      Value QMENU_ITEM_REGULAR (0) means that item is regular item and is not grouped.
         *      Value QMENU_ITEM_CHECKABLE (255) means that item is checkable and should be
         *          interpreted as checkable. Current state is controlled via isChecked() and
         *          setChecked() methods.
         *      Value between 1-254 (including) means that item is radio group item in group of
         *          items with same value. Current state is controlled via isChecked() and
         *          setChecked() methods.
         */
        byte getGroupIndex() {
            return _groupIndex;
        }

        /**
         * @brief Sets item's group index. Setting it, does not affect isChecked() value.
         * @param groupIndex Value of group index to be set. This number meaning is:
         *      Value QMENU_ITEM_REGULAR (0) means that item is regular item and is not grouped.
         *      Value QMENU_ITEM_CHECKABLE (255) means that item is checkable and should be
         *          interpreted as checkable. Current state is controlled via isChecked() and
         *          setChecked() methods.
         *      Value between 1-254 (including) means that item is radio group item in group of
         *          items with same value. Current state is controlled via isChecked() and
         *          setChecked() methods.
         */
        void setGroupIndex(byte groupIndex) {
            _groupIndex = groupIndex;
        }

        /**
         * @brief Gets if the item is regular menu item (not radio or checkable item).
         * @return Returns true if this item is regular item or false if not.
         */
        bool isRegular() {
            return _groupIndex == QMENU_ITEM_REGULAR;
        }

        /**
         * @brief Sets item's type to regular menu item using group index value
         * QMENU_ITEM_REGULAR (0).
         */
        void setRegular() {
            setGroupIndex(QMENU_ITEM_REGULAR);
        }


        /**
         * @brief Gets if the item is radio group item (has set valid radio group index).
         * @return Returns true if this item is radio group item or false if not.
         */
        bool isRadio() {
            return (_groupIndex > QMENU_ITEM_REGULAR) && (_groupIndex < QMENU_ITEM_CHECKABLE);
        }

        /**
         * @brief Gets if the item is checkable menu item.
         * @return Returns true if this item is checkable item or false if not.
         */
        bool isCheckable() {
            return _groupIndex == QMENU_ITEM_CHECKABLE;
        }

        /**
         * @brief Sets this item type to checkable menu item using group index value
         * QMENU_ITEM_CHECKABLE (255).
         * @param value Set to true to set item chechable or false to set item to be regular item.
         */
        void setCheckable(bool value) {
            _groupIndex = value ? QMENU_ITEM_CHECKABLE : QMENU_ITEM_REGULAR;
        }

        /**
         * @brief Gets parent menu item.
         * @return Returns parent menu item reference or NULL if this item is top menu item.
         */
        QMenuItem* getBack() {
            return this->_back;
        }

        /**
         * @brief Gets if this item has submenu.
         * @return Returns if this item has submenu or false if not.
         */
        bool hasSubmenu() {
            return this->_menu != NULL;
        }

        /**
         * @brief Gets first submenu item.
         * @return Returns first menu item reference from submenu or NULL if this item has no
         *      submenu.
         */
        QMenuItem* getMenu() {
            return this->_menu;
        }

        /**
         * @brief Sets first submenu item.
         * @param menu Reference to menu item that has to be first submenu item. Value of NULL
         * clears submenu.
         */
        QMenuItem* setMenu(QMenuItem* menu) {
            this->_menu = menu;
            menu->_back = this;
            return menu;
        }

        /**
         * @brief Get previous menu item in current menu level.
         * @return Returns previous item in current menu level or NULL if this item is first.
         */
        QMenuItem* getPrev() {
            return this->_prev;
        }

        /**
         * @brief Get next menu item in current menu level.
         * @return Returns next item in current menu level or NULL if this item is last.
         */
        QMenuItem* getNext() {
            return this->_next;
        }

        /**
         * @brief Sets next item in durrent menu level.
         * @param menu Reference to menu item that has to be next item after this item in current
         * menu level.
         */
        QMenuItem* setNext(QMenuItem* next) {
            this->_next = next;
            next->_prev = this;
            next->_back = this->_back;
            return next;
        }
};

/** QMenu on onActiveItemChanged event data */
struct QMenuActiveItemChangedEvent {
    const QMenuItem* oldActiveItem;
    const QMenuItem* newActiveItem;
};

/** QMenu on onActiveItemChanged event callback */
typedef void (*QMenuActiveItemChangedCallback) (QMenuActiveItemChangedEvent);

/** QMenu on onItemUtilized event data */
struct QMenuItemUtilizedEvent {
    const QMenuItem* utilizedItem;
};

/** QMenu on onItemUtilized event callback */
typedef void (*QMenuItemUtilizedCallback) (QMenuItemUtilizedEvent);

/**
 * @brief Menu backend controller.
 */
class QMenu {
    private:
        // Root item
        QMenuItem* _root;
        // Curretly active item
        QMenuItem* _active;

        // Events
        QMenuActiveItemChangedCallback _onActiveItemChanged = NULL;
        QMenuItemUtilizedCallback _onItemUtilized = NULL;

    protected:

        /**
         * @brief Calls onActiveItemChanged event if assigned.
         * @param oldItem Previusly active menu item.
         * @oaram newItem Currently active menu item.
         */
        void doOnActiveItemChanged(QMenuItem* oldItem, QMenuItem* newItem) {
            if (_onActiveItemChanged != NULL) {
                QMenuActiveItemChangedEvent event = {
                    oldItem,
                    newItem
                };
                _onActiveItemChanged(event);
            }
        }

        /**
         * @brief Calls onItemUtilized event if assigned.
         * @param item Utilized item.
         */
        void doOnItemUtilized(QMenuItem* item) {
            if (_onItemUtilized != NULL) {
                QMenuItemUtilizedEvent event = {
                    item
                };
                _onItemUtilized(event);
            }
        }

    public:
        /**
         * @brief Creates new instance and initializes it with default root menu item. Root item is
         *      set as active menu item.
         */
        QMenu() {
            _root = new QMenuItem(0, "__ROOT__");
            _active = _root;
        }

        /**
         * @brief Creates new instance and initializes it with root menu item.
         * @param id Root menu item identification. This could be unique integer value, but no
         *      unique test is performed. Root item is set as active menu item.
         * @param caption Root menu item caption.
         */
        QMenu(int id, char* caption) {
            _root = new QMenuItem(id, caption);
            _active = _root;
        }

        /**
         * @brief Gets root menu item.
         * @return Returns menu root item reference. This is never NULL.
         */
        QMenuItem* getRoot() {
            return _root;
        }

        /**
         * @brief Gets currently active menu item.
         * @return Returns active menu item reference.
         */
        QMenuItem* getActive() {
            return _active;
        }

        /**
         * @brief Gets onActiveItemChanged callback.
         * @return Returns onActiveItemChanged callback or NULL if not assigned.
         */
        QMenuActiveItemChangedCallback getOnActiveItemChanged(){
            return _onActiveItemChanged;
        }

        /**
         * @brief Sets onActiveItemChanged callback.
         * @param onActiveItemChanged The onActiveItemChanged callback.
         */
        void setOnActiveItemChanged(QMenuActiveItemChangedCallback onActiveItemChanged) {
            _onActiveItemChanged = onActiveItemChanged;
        }

        /**
         * @brief Gets onItemUtilized callback.
         * @return Returns onItemUtilized callback or NULL if not assigned.
         */
        QMenuItemUtilizedCallback getOnItemUtilized() {
            return _onItemUtilized;
        }

        /**
         * @brief Sets onItemUtilized callback.
         * @param onItemUtilized The onItemUtilized callback.
         */
        void setOnItemUtilized(QMenuItemUtilizedCallback onItemUtilized) {
            _onItemUtilized = onItemUtilized;
        }

        /**
         * @brief Moves to next menu item. Sets currently active item's next item as active menu
         *      item.
         * @return Returns next menu item reference or NULL if there is no next item.
         */
        QMenuItem* next() {
            if (this->_active != NULL) {
                QMenuItem* oldActive = this->_active;
                QMenuItem* newActive = this->_active->getNext();
                if (newActive != NULL) {
                    this->_active = newActive;
                    doOnActiveItemChanged(oldActive, newActive);
                }
                return newActive;
            }

            return NULL;
        }

        /**
         * @brief Moves to previous menu item. Sets currently active item's previous item as active
         *      menu item.
         * @return Returns previous menu item reference or NULL if there is no previous item.
         */
        QMenuItem* prev() {
            if (this->_active != NULL) {
                QMenuItem* oldActive = this->_active;
                QMenuItem* newActive = this->_active->getPrev();      
                if (newActive != NULL) {
                    this->_active = newActive;
                    doOnActiveItemChanged(oldActive, newActive);
                }
                return newActive;
            }

            return NULL;
        }

        /**
         * @brief Moves to submenu. Sets currently active item's first child item as active menu
         *      item.
         * @return Returns first submenu item reference or NULL if there is no submenu.
         */
        QMenuItem* enter() {
            if (this->_active != NULL) {
                QMenuItem* oldActive = this->_active;
                QMenuItem* newActive = this->_active->getMenu();
                if (newActive != NULL) {
                    this->_active = newActive;
                    doOnActiveItemChanged(oldActive, newActive);
                } else {
                    doOnItemUtilized(this->_active);
                }
                return newActive;
            }

            return NULL;
        }

        /**
         * @brief Moves to parent menu. Sets currently active item's parent item as active menu
         *      item.
         * @return Returns parent menu item reference or NULL if there is no parent item.
         */
        QMenuItem* back() {
            if (this->_active != NULL) {
                QMenuItem* oldActive = this->_active;
                QMenuItem* newActive = this->_active->getBack();
                if (newActive != NULL) {
                    this->_active = newActive;
                    doOnActiveItemChanged(oldActive, newActive);
                }
                return newActive;
            }

            return NULL;
        }

        /**
         * @brief Gets top most item from current.
         * @param item Custom menu item.
         * @return Returns top most item in menu or submenu or NULL if given
         *      item is NULL.
         */
        QMenuItem* getTopItem(QMenuItem* item) {
            if (item == NULL) {
                return NULL;
            }

            QMenuItem* top = item;
            while (top->getPrev() != NULL) {
                top = top->getPrev();
            }
            return top;
        }

        /**
         * @brief Finds first item in whole menu identified by given id.
         * @param id Target item id. Note - id must not be unique.
         * @param inTree Set to true to find item in submenu or false to search only on current menu
         * level.
         * @return Returns pointer to first item found with given id or NULL if no item found.
         */
        QMenuItem* find(int id, bool inTree) {
            return find(getRoot(), id, inTree);
        }

        /**
         * @brief Finds first item in whole menu identified by given id.
         * @param root Root item where the searching starts from.
         * @param id Target item id. Note - id must not be unique.
         * @param inTree Set to true to find item in submenu or false to search only on current menu
         * level.
         * @return Returns pointer to first item found with given id or NULL if no item found.
         */
        QMenuItem* find(const QMenuItem* root, int id, bool inTree) {
            QMenuItem* item = root;
            while (item != NULL) {
                // Check this item is target
                if (item->getId() == id) {
                    return item;
                // Try find it in submenu
                } else if (inTree && item->getMenu() != NULL) {
                    QMenuItem* result = find(item->getMenu(), id, inTree);
                    if (result != NULL) {
                        return result;
                    }
                }
                item = item->getNext();
            }

            return NULL;
        }

        /**
         * @brief Sets item's checked state if item is checkable.
         * @param item Item which state has to be set. Item has to be checkable.
         * @return Returns pointer to item that has been checked or unchecked or NULL this item is
         * not checkable.
         */
        QMenuItem* setCheckable(QMenuItem* item, bool checked) {
            if (item != NULL && item->isCheckable()) {
                item->setChecked(checked);
                return item;
            }

            return NULL;
        }

        /**
         * @brief Toggles item's checked state if item is checkable.
         * @param item Item to be toggled. Item has to be checkable.
         * @return Returns pointer to item that has been checked or unchecked or NULL this item is
         * not checkable.
         */
        QMenuItem* toggleCheckable(QMenuItem* item) {
            return item != NULL ? setCheckable(item, !item->isChecked()) : NULL;
        }

        /**
         * @brief Switches radio items in group. Turns given item to be checked and all items at
         * given menu level with same group index to be unchecked.
         * @param switchItem Item to be checked in radio group. This item must be radio item.
         * @return Returns pointer to item that has been checked or NULL if item is not radio item.
         */
        QMenuItem* switchRadio(QMenuItem* switchItem) {
            // If given item is not radio item, invalid call
            if (switchItem == NULL || !switchItem->isRadio()) {
                return NULL;
            }

            // If given item is already checked, nothing to do
            if (switchItem->isChecked()) {
                return switchItem;
            }

            // Get first item in given item's level
            QMenuItem* item = getTopItem(switchItem);
            byte groupIndex = switchItem->getGroupIndex();

            // Loop for all items in level
            while (item != NULL) {

                // Check given item or uncheck others in group
                if (item->isRadio() && item->getGroupIndex() == groupIndex) {
                        item->setChecked(item == switchItem);
                }

                // Next item
                item = item->getNext();
            }

            return switchItem;
        }
};

/** QMenuRenderer onItemRender event data */
struct QMenuRenderItemEvent {
    const QMenuItem* item;
    const boolean isActive;
    const int menuIndex;
    const int renderIndex;
};

/** QMenuRenderer onItemRender event */
typedef void (*QMenuOnRenderItemCallback) (QMenuRenderItemEvent);

/**
 * @brief Rendering machine for menu backend controller.
 */
class QMenuRenderer {
    private:
        QMenuOnRenderItemCallback _onRenderItem;

    protected:
        QMenu* menu;

        /**
         * @brief Calls assigned render callback.
         * @param item Menu item to be rendered.
         * @param isActive Set to true to mark rendered item as curretly selected.
         * @param menuIndex Zero based item index of whole menu.
         * @param renderIndex Zero based item index in menu viewport.
         */
        void renderItem(QMenuItem* item, boolean isActive, int menuIndex, int renderIndex) {
            if (this->_onRenderItem) {
                QMenuRenderItemEvent event = { item, isActive, menuIndex, renderIndex };
                this->_onRenderItem(event);
            }
        }

    public:
        /**
         * @brief Creates new QMenuRenderer instance.
         * @param menu Menu to redner.
         */
        QMenuRenderer(QMenu* menu) {
            this->menu = menu;
        }

        /**
         * @brief Gets render item callback called when menu item has to be rendered.
         * @return Returns onRenderItemCallbackEventCallback function reference or
         * NULL if not assigned.
         */
        QMenuOnRenderItemCallback getOnRenderItem() {
            return this->_onRenderItem;
        }

        /**
         * @brief Sets render item callback called when menu item has to be rendered.
         * @param The onRenderItemCallbackEventCallback function.
         */
        void setOnRenderItem(QMenuOnRenderItemCallback onRenderItemCallback) {
            this->_onRenderItem = onRenderItemCallback;
        }
};

/**
 * @brief List menu renderer.
 */
class QMenuListRenderer : public QMenuRenderer {
    private:
        int _viewportIndex = 0;
        int _viewportSize;

    protected:
        /**
         * @brief Calculates index of menu item in viewport.
         * @param top First item of current submenu.
         * @param active Currently active menu item.
         */
        void calcViewportIndex(QMenuItem* top, QMenuItem* active) {
            int index = 0;
            QMenuItem* item = top;
            while (item != NULL) {

                // Check active item in viewport
                if (item == active) {

                    // Active item before view port
                    if (index < this->_viewportIndex) {
                        this->_viewportIndex = index;
                        break;
                    }

                    // Active item after viewport
                    if (index >= this->_viewportIndex + this->_viewportSize) {
                        this->_viewportIndex = index - this->_viewportSize + 1;
                        break;
                    }
                }

                // Next item
                item = item->getNext();
                index++;
            }
        }

        /**
         * @brief Calls rendering for all items in viewport.
         * @param top Top item of rendered menu level.
         * @param active Currently active item in rendered menu level.
         */
        void renderItemsInViewport(QMenuItem* top, QMenuItem* active) {
            int index = 0;
            QMenuItem* item = top;
            while (item != NULL) {

                // Render item if in viewport
                if (index >= this->_viewportIndex) {
                    renderItem(item, item == active, index, index - this->_viewportIndex);
                }

                // All renderred, break
                if (index >= this->_viewportIndex + this->_viewportSize - 1) {
                    break;
                }

                // Next item
                item = item->getNext();
                index++;
            }
        }

    public:
        /**
         * @brief Creates new QMenuRenderer instance.
         * @param menu Menu to redner.
         * @param viewportSize Number of menu items mutually visible.
         */
        QMenuListRenderer(QMenu* menu, int viewportSize)
            : QMenuRenderer(menu)
        {
            this->_viewportSize = viewportSize;
        }

        /**
         * @brief Renders associated menu. Calculates items that has to be rendered to current
         * viewport and calls callback to provide user defined item
         * draw.
         */
        void render() {
            // Check menu set
            if (this->menu == NULL) {
                return;
            }

            // Check active set
            QMenuItem* active = this->menu->getActive();
            if (active == NULL) {
                return;
            }

            // Get first item in current menu
            QMenuItem* top = this->menu->getTopItem(active);

            // Calc viewport position
            calcViewportIndex(top, active);

            // Render viewport
            renderItemsInViewport(top, active);
        }
};

#endif
