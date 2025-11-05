@description('Name of the Function App')
param functionAppName string

@description('Location for resources (must support Flex Consumption)')
@allowed([
  'EastUS2'
  'WestEurope'
  'NorthEurope'
  'CentralUS'
  'UKSouth'
])
param location string

@description('Environment name for uniqueness')
param environmentName string

// Generate unique suffix using resource group, function name, location, and environment
var randomSuffix = uniqueString(resourceGroup().id, functionAppName, location, environmentName)
var storageAccountName = toLower('st${substring(randomSuffix, 0, min(18, length(randomSuffix)))}')

// Storage Account
resource storage 'Microsoft.Storage/storageAccounts@2022-09-01' = {
  name: storageAccountName
  location: location
  sku: {
    name: 'Standard_LRS'
  }
  kind: 'StorageV2'
}

// Flex Consumption Plan with scaling rules
resource plan 'Microsoft.Web/serverfarms@2022-03-01' = {
  name: '${functionAppName}-plan'
  location: location
  sku: {
    name: 'FC1'
    tier: 'FlexConsumption'
  }
  properties: {
    maximumElasticWorkerCount: 20 // Optional: max scale-out instances
    perSiteScaling: false
  }
}

// Application Insights for monitoring
resource appInsights 'Microsoft.Insights/components@2020-02-02' = {
  name: '${functionAppName}-ai'
  location: location
  kind: 'web'
  properties: {
    Application_Type: 'web'
  }
}

// Function App (.NET Isolated)
resource functionApp 'Microsoft.Web/sites@2022-03-01' = {
  name: functionAppName
  location: location
  kind: 'functionapp'
  properties: {
    reserved: true // Linux required
    serverFarmId: plan.id
    siteConfig: {
      linuxFxVersion: 'DOTNET-ISOLATED|9.0' // Target .NET 9 isolated runtime
      appSettings: [
        {
          name: 'AzureWebJobsStorage'
          value: 'DefaultEndpointsProtocol=https;AccountName=${storage.name};AccountKey=${storage.listKeys().keys[0].value}'
        }
        {
          name: 'FUNCTIONS_EXTENSION_VERSION'
          value: '~4'
        }
        {
          name: 'FUNCTIONS_WORKER_RUNTIME'
          value: 'dotnet-isolated'
        }
        {
          name: 'APPINSIGHTS_INSTRUMENTATIONKEY'
          value: appInsights.properties.InstrumentationKey
        }
      ]
    }
  }
}

// Outputs
output storageAccountName string = storage.name
output functionAppName string = functionApp.name
output appInsightsName string = appInsights.name
