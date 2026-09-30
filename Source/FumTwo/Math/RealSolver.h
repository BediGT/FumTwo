// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include <Engine/HitResult.h>

class IRealProjectileInterface;
class IRealTargetInterface;

class RealSolver
{ 
public:
	static void Solve(const IRealProjectileInterface* Projectile, const IRealTargetInterface* Target, const FHitResult& HitResult);
};
