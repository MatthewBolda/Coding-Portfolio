using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.AI;


public class ZombieAI : MonoBehaviour
{
    /////////////////////////////////
    ///    Movement  Varaibles    ///
    /////////////////////////////////
    NavMeshAgent nm;
    public Transform target;

    public enum AIState { Idle, Chasing}
    public AIState aiState = AIState.Idle;

    public float distanceForChasing = 10f;
    public float distanceForIdle = 15f;

    /////////////////////////////////
    ///   Health Bar Variables    ///
    /////////////////////////////////
    public int maxHealth = 100;
    public int currentHealth;
    public HealthBarSlider healthBar;

    // Start is called before the first frame update
    void Start()
    {
        currentHealth = maxHealth;
        nm = GetComponent<NavMeshAgent>();
        StartCoroutine(Think());

    }

    /////////////////////////////////
    ///      Movement Logic       ///
    /////////////////////////////////
    IEnumerator Think()
    {
        while (true)
        {
            switch (aiState)
            {
                case AIState.Idle:
                    float dist = Vector3.Distance(target.position, transform.position);
                    if (dist < distanceForChasing)
                    {
                        aiState = AIState.Chasing;
                    }
                    nm.SetDestination(transform.position);
                    break;
                case AIState.Chasing:
                    dist = Vector3.Distance(target.position, transform.position);
                    if (dist > distanceForIdle)
                    {
                        aiState = AIState.Idle;
                    }
                    nm.SetDestination(target.position);
                    break;
                default:
                    break;
            }
            yield return new WaitForSeconds(1f);
        }
    }


    /////////////////////////////////
    ///        Damage Logic       ///
    /////////////////////////////////
    public void TakeDamage(int amount)
    {
        currentHealth -= amount;
        healthBar.SetHealth(currentHealth);
        if (currentHealth <= 0)
        {
            Die();
        }
    }

    void Die()
    {
        Destroy(gameObject);
    }
}
