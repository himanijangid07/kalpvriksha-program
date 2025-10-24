#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Product {
    int productId;
    char productName[50];
    float price;
    int quantity;
};

void addNewProduct(struct Product **products, int *count) {
    *products = (struct Product *)realloc(*products, (*count + 1) * sizeof(struct Product));

    if(*products == NULL) {
        printf("Memory allocation failed.\n");
        return;
    }

    printf("Enter new Product Details: \n");

    printf("Product ID: ");
    scanf("%d", &(*products)[*count].productId);
    getchar();

    printf("Product Name: ");
    fgets((*products)[*count].productName, sizeof((*products)[*count].productName), stdin);
    (*products)[*count].productName[strcspn((*products)[*count].productName, "\n")] = 0;

    printf("Product Price: ");
    scanf("%f", &(*products)[*count].price);

    printf("Product Quantity: ");
    scanf("%d", &(*products)[*count].quantity);

    printf("Product added successfully!");

    (*count)++;
}

void displayProducts(struct Product *products, int count) {
    printf("========= PRODUCT LIST =========\n");

    for(int index = 0; index < count; index++) {
        printf("Product ID: %d  |  Product Name: %s  |  Product Price: %.2f  |  Product Quantity: %d\n", products[index].productId, products[index].productName, products[index].price, products[index].quantity);
    }

    printf("\n");
}

void updateQuantity(struct Product *products, int count) {
    int id, found = 0, newQuantity;

    printf("Enter product ID to update quantity: ");
    scanf("%d", &id);
    
    for(int index = 0; index < count; index++) {
        if(id == products[index].productId) {
            printf("Enter new Quantity: ");
            scanf("%d", &newQuantity);
            products[index].quantity = newQuantity;
            printf("Quantity updated successfully!\n");
            found = 1;
            break;
        }
    }

    if(!found) {
        printf("ID not found\n");
    }

    printf("\n");
}

void searchProductByID(struct Product *products, int count) {
    int id, found = 0;

    printf("Enter product ID to search: ");
    scanf("%d", &id);

    for(int index = 0; index < count; index++) {
        if(id == products[index].productId) {
            printf("Product found: Product ID: %d  |  Product Name: %s  |  Product Price: %.2f  |  Product Quantity: %d\n", products[index].productId, products[index].productName, products[index].price, products[index].quantity);
            found = 1;
            break;
        }
    }

    if(!found) {
        printf("Product not found\n");
    }

    printf("\n");
}

int isSubString(char *text, char *pattern) {
    int firstIndex, secondIndex;
    
    for(firstIndex = 0; text[firstIndex] != '\0'; firstIndex++) {
        for(secondIndex = 0; pattern[secondIndex] != '\0'; secondIndex++) {
            if(text[firstIndex + secondIndex] != pattern[secondIndex]) {
                break;
            }
        }
        if(pattern[secondIndex] == '\0') {
            return 1;
        }
    }
    return 0;
}

void searchProductByName(struct Product *products, int count) {
    char name[50];
    int found = 0;

    printf("Enter Name to search (Partial allowed): ");
    fgets(name, sizeof(name), stdin);
    name[strcspn(name, "\n")] = 0;

    for(int index = 0; index < count; index++) {
        if(isSubString(products[index].productName, name)) {
            printf("Products Found: Product ID: %d  |  Product Name: %s  |  Product Price: %.2f  |  Product Quantity: %d\n", products[index].productId, products[index].productName, products[index].price, products[index].quantity);
            found = 1;
        }
    }

    if(!found) {
        printf("Product not found\n");
    }

    printf("\n");
}

void searchProductByPriceRange(struct Product *products, int count) {
    float minimumPrice, maximumPrice;
    int found = 0;

    printf("Enter minimum price: ");
    scanf("%f", &minimumPrice);

    printf("Enter maximum price: ");
    scanf("%f", &maximumPrice);

    printf("Products in price range: \n");

    for(int index = 0; index < count; index++) {
        if(products[index].price >= minimumPrice && products[index].price <= maximumPrice) {
            printf("Product ID: %d  |  Product Name: %s  |  Product Price: %.2f  |  Product Quantity: %d\n", products[index].productId, products[index].productName, products[index].price, products[index].quantity);
            found = 1;
        }
    }

    if(!found) {
        printf("Products not found\n");
    }
    
    printf("\n");
}

void deleteProduct(struct Product **products, int *count) {
    int id, index, found = 0;

    printf("Enter product ID to delete: ");
    scanf("%d", &id);

    for(index = 0; index < *count; index++) {
        if((*products)[index].productId == id) {
            found = 1;
            break;
        }
    }

    if(!found) {
        printf("Product not found\n");
    }

    for(int iterator = index; iterator < *count - 1; iterator++) {
        (*products)[iterator] = (*products)[iterator + 1];
    }

    (*count)--;

    *products = (struct Product *)realloc(*products, (*count) * sizeof(struct Product));

    printf("Product deleted successfully!\n");
    printf("\n");
}

int main() {
    int numberOfProducts, index, choice;

    printf("Enter the initial number of products: ");
    if (scanf("%d", &numberOfProducts) != 1 || numberOfProducts < 0){
        printf("Invalid input. Please enter a non-negative integer. \n");
        return 1;
    }

    struct Product *products = (struct Product *)calloc(numberOfProducts, sizeof(struct Product));
    if(products == NULL) {
        printf("Memory allocation failed\n");
        return 1;
    }

    for(index = 0; index < numberOfProducts; index++) {
        printf("Enter details for product %d: \n", index + 1);

        printf("Product ID: ");
        scanf("%d", &products[index].productId);
        getchar();

        printf("Product Name: ");
        fgets(products[index].productName, sizeof(products[index].productName), stdin);
        products[index].productName[strcspn(products[index].productName, "\n")] = 0;

        printf("Product Price: ");
        scanf("%f", &products[index].price);

        printf("Product Quantity: ");
        scanf("%d", &products[index].quantity);
    }

    do {
        printf("========= INVENTORY MENU =========\n");
        printf("1. Add New Product\n");
        printf("2. View All Products\n");
        printf("3. Update Quantity\n");
        printf("4. Search Product By ID\n");
        printf("5. Search Product By Name\n");
        printf("6. Search Product By Price Range\n");
        printf("7. Delete Product\n");
        printf("8. Exit\n");

        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar();
        printf("\n");

        switch (choice) {
            case 1:
                addNewProduct(&products, &numberOfProducts);
                break;
            case 2:
                displayProducts(products, numberOfProducts);
                break;
            case 3:
                updateQuantity(products, numberOfProducts);
                break;
            case 4:
                searchProductByID(products, numberOfProducts);
                break;
            case 5:
                searchProductByName(products, numberOfProducts);
                break;
            case 6:
                searchProductByPriceRange(products, numberOfProducts);
                break;
            case 7:
                deleteProduct(&products, &numberOfProducts);
                break;
            case 8:
                free(products);
                printf("Memory released successfully. Exiting program......\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while(choice != 8);

    return 0;
}