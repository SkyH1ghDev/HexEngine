#pragma once

class DX12FeatureValidator
{
public:
    DX12FeatureValidator() = default;
    ~DX12FeatureValidator() = default;
    DX12FeatureValidator(const DX12FeatureValidator&) = delete;
    DX12FeatureValidator& operator=(const DX12FeatureValidator&) = delete;
    DX12FeatureValidator(DX12FeatureValidator&&) = delete;
    DX12FeatureValidator& operator=(DX12FeatureValidator&&) = delete;
    
    bool CheckTearingSupport();
    
private:
    
};
